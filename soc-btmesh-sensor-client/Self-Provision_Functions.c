#include <stdio.h>
#include <stdint.h>
#include <pthread.h>
#include "math.h"
#include "native_gecko.h"
#include "Self-Provision_Functions.h"
#include "app.h"
#include "retargetserialhalconfig.h"
#include "TID.h"

/* My Keys*/
aes_key_128 NetKey = {{0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01}};
aes_key_128 AppKey = {{0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02}};
aes_key_128 DevKey = {{0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03}};


void Self_Provision_Device(void)
{
	//Set Keys
	
	uint16 result=0;
		
	result = gecko_cmd_mesh_node_set_provisioning_data(DevKey, NetKey, 0, 0, Unicast, 0)->result;
	printf("gecko_cmd_mesh_node_set_provisioning_data %x\n\r", result);
	
	result = gecko_cmd_mesh_test_add_local_key(1, AppKey,0,0)->result;
	printf("gecko_cmd_mesh_test_add_local_key %x\n\r", result);

	gecko_cmd_hardware_set_soft_timer(2 * ONE_SECOND, TIMER_ID_RESTART, 1); //Restart device to ensure changes are implimented.
	
	// Bind Models
	
	result = gecko_cmd_mesh_test_bind_local_model_app(0, 0, 0xffff, GENERIC_LEVEL_SERVER)->result;
	printf("GENERIC_LEVEL_SERVER: gecko_cmd_mesh_test_bind_local_model_app %x\n\r", result);

	result = gecko_cmd_mesh_test_bind_local_model_app(0, 0, 0xffff, GENERIC_LEVEL_CLIENT)->result;
	printf("GENERIC_LEVEL_CLIENT: gecko_cmd_mesh_test_bind_local_model_app %x\n\r", result);

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


	//set Relay
	result = gecko_cmd_mesh_test_set_relay(1, 2, 0)->result;
	printf("Set Relay gecko_cmd_mesh_test_set_relay %x\n\r", result);

	struct gecko_msg_mesh_test_get_relay_rsp_t *reps = gecko_cmd_mesh_test_get_relay();
	printf("gecko_cmd_mesh_test_get_relay returns %x for enabled \r\n", reps->enabled);

	//set Network
	result = gecko_cmd_mesh_test_set_nettx(2, 0)->result;
	printf("Set Relay gecko_cmd_mesh_test_set_nettx %x\n\r", result);

	/* Set Proxy */
	uint8_t ProxyValue = 1;
	result = gecko_cmd_mesh_test_set_local_config(mesh_node_gatt_proxy,0,sizeof(ProxyValue), &ProxyValue);

	

}
