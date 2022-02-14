#include "libs/http/include/HttpModule.hpp"
#include "libs/http/include/RequestOutputQueue.hpp"
#include "libs/http/include/ResponseInputQueue.hpp"

/**
 * Please note that this example will not be the same when the config parser will be
 * available
 */

int main()
{
    modules::HttpModule http(3000);
    modules::ResponseInputQueue responses;
    modules::RequestOutputQueue requests;

    std::cout << "Hello, World!" << std::endl;
    http.Run(requests, responses);
    return 0;
}
