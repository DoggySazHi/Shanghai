#ifndef SHANGHAI_NOTIFICATIONSERVER_H
#define SHANGHAI_NOTIFICATIONSERVER_H

#include <functional>
#include <string>

struct sd_bus;
struct sd_bus_message;
struct sd_bus_error;

// Very rudementary (and very non-compliant) singleton implementation of org.freedesktop.Notifications
class NotificationServer {
public:
    static bool init(std::function<void(const std::string&)> onSummary);
    static void poll();
    static void shutdown();

    // Implementations of the DBus notification object
    static int handleNotify(sd_bus_message* m, void* userdata, sd_bus_error* retError);
    static int handleGetCapabilities(sd_bus_message* m, void* userdata, sd_bus_error* retError);
    static int handleGetServerInformation(sd_bus_message* m, void* userdata, sd_bus_error* retError);

private:
    static sd_bus* bus;
    static std::function<void(const std::string&)> summaryCallback;
};

#endif //SHANGHAI_NOTIFICATIONSERVER_H
