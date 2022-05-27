#include <drogon/drogon.h>
using namespace drogon;
int main() {
    app()
        .setLogPath("./")
        .setLogLevel(trantor::Logger::kWarn)
        .addListener("0.0.0.0", 80)
        .setThreadNum(16)
        .enableRunAsDaemon()
        .run();
}
