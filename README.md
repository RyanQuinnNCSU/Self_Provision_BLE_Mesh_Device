# Self_Provision_BLE_Mesh_Device
> Creating Self-Provisioned BLE Mesh firmware build from Silabs example project "soc-btmesh-sensor-client". 


##  Why Use Self-Provisioning
Devloping self-provisioning firmware for BLE Mesh Devices allows for the rapid development of test networks without need a provsioner (usually in the form of a mobile app) to set the network configuration. This is often used in product/network testing, but as Silabs mentions in there API reference material, this should not be used for offical products released to the public. 

## Setup Requrements
To create the base project you will need to have Silabs' Simplicity Studio and the BLE Mesh Stack downloaded (as you need for any BLE Mesh firmware). For hardware you will need to BLE Mesh capable devices with Silabs chips. I use dev kits with the efr32mg12p432f1024gl125 chip.

To create a base project in Simplcity Studio, go to the Launcher tab and click on New Project. Select the Project that you think best suits you needs, I choose "soc-btmesh-sensor-client". 

## Creating a Self-Provisioned Network:
With the base project created, The following code changes need to be added inorder to create a self-provisioning network.

### Enable Mesh Test APIs

In order to pre-configure network details to your devices you will need to enable the test APIs with  ```gecko_bgapi_class_mesh_test_init();``` 
Add this with the other API intit functions contained in ```gecko_bgapi_classes_init()``` (see soc-btmesh-sensor-client/app.c).


### Create Unique Unicast Addresses
Each device in the BLE Mesh network needs to have a unique unicast address. If not you risk having mesh messages getting dropped. While there is nothing wrong with creating seperate firmware images for each device (each defining a different 16 bit unicast value), it may be easier to just use the last 4 bytes of the mac address. This will allow one firmware image to be used for all network devices.

The MAC address of each device can be referenced using ```SYSTEM_GetUnique()```. One thing to note, the most significant bit of the unicast address must be 0. Twice now I for got about this and had failures based on my device's MAC address. The solution is to only use the last 15 (not 16) of the MAC for your unicast. For example ```Unicast = (uint16_t)(SYSTEM_GetUnique() & 0x7FFF);``` (soc-btmesh-sensor-client/main.c).

One final note. ```#include "em_system.h"``` must be in added for ```SYSTEM_GetUnique``` to work. 

### Set Network Keys
To set the Network and Device Key use ```gecko_cmd_mesh_node_set_provisioning_data(DevKey, NetKey, 0, 0, Unicast, 0)```. 

To set the App Key use ```gecko_cmd_mesh_test_add_local_key(1, AppKey,0,0)```. 

The definition of ```Unicast``` was shown above. The Net, App, and Dev Keys are all aes_key_128 data type. Below are the keys used in this example code. 
```
/* My Keys*/
aes_key_128 NetKey = {{0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01}};
aes_key_128 AppKey = {{0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02}};
aes_key_128 DevKey = {{0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03}};
```
(see soc-btmesh-sensor-client/Self-Provision_Functions.c)

### Configure Models
The models required for each project depend on the end application. For this example code, Generic_Client and Generic_Server are used. The following sub sections break down the steps required to configure a model.  

#### Update Project Config File
Regardless if a network is self-provisioned or not, any models not included in the default configuration of the base projected need to be added in using the isc file (soc-btmesh-sensor-client/soc-btmesh-sensor-client.isc). It is assumed users of this repo are fimiliar with how to use the isc file to generate the necessary code changes to add new models to there project. The goal of this section is to be a reminder to do so.

#### Init Functions
Again, regardless if a newtwork is self-provisioned or not, some models will require an init function to be called inorder to function. In the case of the generic server and client models these are ```gecko_cmd_mesh_generic_client_init()``` and ```gecko_cmd_mesh_generic_server_init()``` (see soc-btmesh-sensor-client/app.c).

#### Bind Models
To bind your models use the ```gecko_cmd_mesh_test_bind_local_model_app()``` API. 

Example: 
```
// Bind Models
	
	result = gecko_cmd_mesh_test_bind_local_model_app(0, 0, 0xffff, GENERIC_LEVEL_SERVER)->result;
	printf("GENERIC_LEVEL_SERVER: gecko_cmd_mesh_test_bind_local_model_app %x\n\r", result);

	result = gecko_cmd_mesh_test_bind_local_model_app(0, 0, 0xffff, GENERIC_LEVEL_CLIENT)->result;
	printf("GENERIC_LEVEL_CLIENT: gecko_cmd_mesh_test_bind_local_model_app %x\n\r", result);
```
(see soc-btmesh-sensor-client/Self-Provision_Functions.c)

#### Setup Publish and Subscribe Settings
To set the publish and sub address for each model use the APIs ```gecko_cmd_mesh_test_set_local_model_pub()``` and ```gecko_cmd_mesh_test_add_local_model_sub()```. 

Example: 
```
//Set Models Pub Address

	result = gecko_cmd_mesh_test_set_local_model_pub(0, 0, 0xffff, GENERIC_LEVEL_SERVER, SERVER_PUB_ADD, 5, 0, 3, 0)->result;
	printf("SENSOR_CLIENT gecko_cmd_mesh_test_set_local_model_pub %x\n\r", result);
	result = gecko_cmd_mesh_test_set_local_model_pub(0, 0, 0xffff, GENERIC_LEVEL_CLIENT, CLIENT_PUB_ADD, 5, 0, 3, 0)->result; //Mulitcast = SERVER_PUB_ADD
	printf("GENERIC_LEVEL_CLIENT gecko_cmd_mesh_test_set_local_model_pub %x\n\r", result);

	//Set Models Sub Address

	result = gecko_cmd_mesh_test_add_local_model_sub(0, 0xffff, GENERIC_LEVEL_SERVER, SERVER_SUB_ADD)->result;
	printf("LIGHT_LIGHTNESS_CLIENT gecko_cmd_mesh_test_add_local_model_sub %x\n\r", result);

	result = gecko_cmd_mesh_test_add_local_model_sub(0, 0xffff, GENERIC_LEVEL_CLIENT, CLIENT_SUB_ADD)->result;
	printf(" vendor gecko_cmd_mesh_test_add_local_model_sub %x\n\r", result);
```
(see soc-btmesh-sensor-client/Self-Provision_Functions.c)

### Publishing and Recieving Messages
In this build, each device acts as a client and server. Using an internal timer, the client model will publish a generic level set request every 10 seconds using ```gecko_cmd_mesh_generic_client_publish()```.

```
case TIMDER_ID_SEND_GENERIC_CLIENT_MESSAGE:
    	printf("Sending Generic Client Message. \r\n");
    	uint16_t pub_res;
    	uint8_t byte_array[2] = {0xAA,0xBB};
    	pub_res = gecko_cmd_mesh_generic_client_publish((uint16)GENERIC_LEVEL_CLIENT, (uint16)ELEM_0, (uint8)Transaction_id, (uint32)TRANS, (uint16)DELAY, (uint16)CLIENT_FLAGS, (uint8)MESH_GENERIC_CLIENT_request_level, (uint8)DATA_LENGHT_CLIENT, byte_array)->result;
    	printf("Pub Result = %x \n", pub_res);
    	if(Transaction_id < 255){
    	    						Transaction_id++;
    	    					}
    	    					else{
    	    						Transaction_id = 0;
    	    					}
          break;
```


When another devices server in this network receives the client request, a ```gecko_evt_mesh_generic_server_client_request_id``` event will be triggered. In this example, upon receving a client request a server response is sent via ```gecko_cmd_mesh_generic_server_response()```. Note that the firmware checks that the client message isn`t from it`s own unicast (this confirms message is from another device).

```
case gecko_evt_mesh_generic_server_client_request_id:
    	printf("Request From Client Received! \r\n");
    	struct gecko_msg_mesh_generic_server_client_request_evt_t *client_req =(struct gecko_msg_mesh_generic_server_client_request_evt_t *) &(pEvt->data);
    	uint16_t client_addr = client_req->client_address;
    	if( client_addr != Unicast){
			printf("Client Node```s Address: %x \r\n", client_addr);
			printf("Sending Server Response. \r\n");
			uint16_t pub_res;
			uint8_t byte_array[4] = {0xAF,0xBF,0xCF,0xDF};
			pub_res = gecko_cmd_mesh_generic_server_response((uint16)GENERIC_LEVEL_SERVER, (uint16)ELEM_0, (uint16)SERVER_PUB_ADD, (uint16)APP_KEY_INDEX_0, (uint16)DELAY, (uint16)SERVER_FLAGS, (uint8)MESH_GENERIC_CLIENT_request_level, (uint8)DATA_LENGHT_SERVER, byte_array)->result;
			printf("Pub Result = %x \r\n", pub_res);
    	}
    	break;
```

Finally, the client will recieve the servers response as a ```gecko_evt_mesh_generic_client_server_status_id``` event. In this example I print the data sent from the server```s response onto the console.

```
case gecko_evt_mesh_generic_client_server_status_id:
    	printf("Server Response Received! \r\n");
    	struct gecko_msg_mesh_generic_client_server_status_evt_t *server_resp =(struct gecko_msg_mesh_generic_client_server_status_evt_t *) &(pEvt->data);
    	uint16_t server_addr = server_resp->server_address;
    	printf("Server Node```s Address: %x \r\n", server_addr);
    	//read message data
    	printf("Server Data Bytes: ");
    	for( int di = 0; di < 4; di++){
    		printf("%x", server_resp->parameters.data[di]);
    	}
    	printf("\r\n\n");
    	break;
```

### Seeing the Code in Action
Once the firmware has been flash to your devices, check to the serial log to see what the devices is doing. The serial log shown below is from one of my 2 devices flashed with this firmware. 

Note that many lines have a number beside the command implimented. This number is the result code for executing the API call. If this values is a non-zero value, then an error has occured. 

1. Device boots up, Device name is set and node initialize event is triggered:
```
Device name: ```sensor client 95:0b
node initialized
```

2.  Set Network, Device and Aplication Keys:
```
gecko_cmd_mesh_node_set_provisioning_data 0
gecko_cmd_mesh_test_add_local_key 0
```

3. Bind Models:
```
GENERIC_LEVEL_SERVER: gecko_cmd_mesh_test_bind_local_model_app 0
GENERIC_LEVEL_CLIENT: gecko_cmd_mesh_test_bind_local_model_app 0
```

4. Set publish and subscribe settings:
```
GENERIC_LEVEL_SERVER: gecko_cmd_mesh_test_bind_local_model_app 0
GENERIC_LEVEL_CLIENT: gecko_cmd_mesh_test_bind_local_model_app 0
SENSOR_CLIENT gecko_cmd_mesh_test_set_local_model_pub 0
GENERIC_LEVEL_CLIENT gecko_cmd_mesh_test_set_local_model_pub 0
LIGHT_LIGHTNESS_CLIENT gecko_cmd_mesh_test_add_local_model_sub 0
```

5. Set network relay and retransmit settings:
```
Set Relay gecko_cmd_mesh_test_set_relay 0
gecko_cmd_mesh_test_get_relay returns 1 for enabled
Set Relay gecko_cmd_mesh_test_set_nettx 0
```

6. Node reboots and initializes as provisioned:
```
got new network key with index 0
Device name: ```sensor client 95:0b```
node initialized
node is provisioned. address:150b, ivi:0
```

7. Initialize generic client and server:
```
mesh_generic_client_init 0
mesh_generic_server_init 0
```

8. Pubish client request message:
```
Sending Generic Client Message.
Pub Result = 0
```

9. Receive response from server:
```
Server Response Received!
Server Node```s Address: 30c1
Server Data Bytes: afbfcfdf
```

10. Receiving a request from another client:
```
Client Node```s Address: 30c1
Sending Server Response.
```

11: Sending server response: 
```
Sending Server Response.
Pub Result = 0
```

# Wrap Up
Hope this helps, best of luck!

# Contact Info
Email: rtquinn2@ncsu.edu
