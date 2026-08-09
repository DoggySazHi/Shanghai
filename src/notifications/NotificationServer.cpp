#include "NotificationServer.h"

#include <cstring>
#include <iostream>
#include <systemd/sd-bus.h>

sd_bus* NotificationServer::bus = nullptr;
std::function<void(const std::string&)> NotificationServer::summaryCallback;

int NotificationServer::handleNotify(sd_bus_message* m, [[maybe_unused]] void* userdata, sd_bus_error* retError) {
    const char* appName = nullptr;
    uint32_t replacesId = 0;
    const char* appIcon = nullptr;
    const char* summary = nullptr;

    // "susssasa{sv}i"
    // (app_name, replaces_id, app_icon, summary, body, actions, hints, expire_timeout)
    // Only pull summary for now
    if (const int result = sd_bus_message_read(m, "suss", &appName, &replacesId, &appIcon, &summary); result < 0) {
        std::cerr << "Failed to parse Notify message: " << strerror(-result) << '\n';
        return sd_bus_reply_method_errno(m, -result, retError);
    }

    if (summaryCallback) {
        summaryCallback(std::string(summary != nullptr ? summary : ""));
    }

    // Notification id; always 1 since we never track/replace/close notifications
    return sd_bus_reply_method_return(m, "u", 1);
}

int NotificationServer::handleGetCapabilities(sd_bus_message* m, [[maybe_unused]] void* userdata, [[maybe_unused]] sd_bus_error* retError) {
    // We don't support anything!
    return sd_bus_reply_method_return(m, "as", 0);
}

int NotificationServer::handleGetServerInformation(sd_bus_message* m, [[maybe_unused]] void* userdata, [[maybe_unused]] sd_bus_error* retError) {
    // (name, vendor, version, spec_version)
    // notify-send fails if we don't respond with this
    return sd_bus_reply_method_return(m, "ssss", "Shanghai", "Shanghai", "1.0", "1.2");
}

static const sd_bus_vtable notificationVtable[] = {
    SD_BUS_VTABLE_START(0),
    SD_BUS_METHOD("Notify", "susssasa{sv}i", "u", NotificationServer::handleNotify, SD_BUS_VTABLE_UNPRIVILEGED),
    SD_BUS_METHOD("GetCapabilities", "", "as", NotificationServer::handleGetCapabilities, SD_BUS_VTABLE_UNPRIVILEGED),
    SD_BUS_METHOD("GetServerInformation", "", "ssss", NotificationServer::handleGetServerInformation, SD_BUS_VTABLE_UNPRIVILEGED),
    SD_BUS_VTABLE_END
};

bool NotificationServer::init(std::function<void(const std::string&)> onSummary) {
    summaryCallback = std::move(onSummary);

    int result = sd_bus_open_user(&bus);
    if (result < 0) {
        std::cerr << "Failed to open user D-Bus, notification server disabled: " << strerror(-result) << '\n';
        bus = nullptr;
        return false;
    }

    result = sd_bus_add_object_vtable(bus, nullptr, "/org/freedesktop/Notifications", "org.freedesktop.Notifications", notificationVtable, nullptr);
    if (result < 0) {
        std::cerr << "Failed to register Notifications object, notification server disabled: " << strerror(-result) << '\n';
        sd_bus_unref(bus);
        bus = nullptr;
        return false;
    }

    result = sd_bus_request_name(bus, "org.freedesktop.Notifications", 0);
    if (result < 0) {
        std::cerr << "Failed to acquire org.freedesktop.Notifications (is another notification daemon running?), notification server disabled: " << strerror(-result) << '\n';
        sd_bus_unref(bus);
        bus = nullptr;
        return false;
    }

    std::cout << "Registered as org.freedesktop.Notifications\n";
    return true;
}

void NotificationServer::poll() {
    if (bus == nullptr) {
        return;
    }

    int result;
    do {
        result = sd_bus_process(bus, nullptr);
    } while (result > 0);

    if (result < 0) {
        std::cerr << "sd_bus_process failed: " << strerror(-result) << '\n';
    }
}

void NotificationServer::shutdown() {
    if (bus != nullptr) {
        sd_bus_unref(bus);
        bus = nullptr;
    }
}
