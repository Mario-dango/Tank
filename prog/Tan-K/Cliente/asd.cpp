/*
#include <stdio.h>
using namespace std;

#include "XmlRpcClient.h"
#include "XmlRpcSocket.h"
#include "XmlRpc.h"
using namespace XmlRpc;

const char host[] = "localhost";
const char url[] = "http://localhost:8891";
const int port = 8891;

int main (){
    XmlRpcClient client (host, port);
}




*/

#include <xmlrpc-c/base.hpp>
#include <xmlrpc-c/client_simple.hpp>

int
main(int argc, char **argv) {

    string const serverUrl("http://localhost:8080/RPC2");
    string const methodName("sample.add");

    xmlrpc_c::clientSimple myClient;
    xmlrpc_c::value result;
        
    myClient.call(serverUrl, methodName, "ii", &result, 5, 7);

    int const sum((xmlrpc_c::value_int(result)));
        // Assume the method returned an integer; throws error if not

    cout << "Result of RPC (sum of 5 and 7): " << sum << endl;

    return 0;
}