#!/usr/bin/env python

#---------------------------------------------------------------------------------------
# Load configuration values
#
#---------------------------------------------------------------------------------------

# see https://wiki.python.org/moin/ConfigParserShootout
from configobj import ConfigObj

config = ConfigObj('RelayshieldMqttController.cfg')

print("{0}".format("Relayshield MQTT Controller"))


#---------------------------------------------------------------------------------------
# Modules and methods to support MQTT
#
#---------------------------------------------------------------------------------------

# requires paho-mqtt package to support MQTT
# see https://pypi.python.org/pypi/paho-mqtt/1.1
# for more information about package
# see http://www.hivemq.com/blog/mqtt-client-library-paho-python

import paho.mqtt.client as mqtt

def on_connect(client, userdata, flags, rc) :
	""" Callback for when the client receives a CONNACK response from the server. """
	print("Connected with result code "+str(rc))
	if (rc == 0) :
		# Subscribing in on_connect() means that if the connection is lost
		# the subscriptions will be renewed when reconnecting.
		print("Subscribing to topics ...")
		for topic in config['mqtt_topics']['TOPICS'] :
			#	topic = config['mqtt_topics']['TOPIC']
			client.subscribe(topic)
			print("{0}".format(topic))
	else :
		tidyupAndExit();

def on_subscribe(client, userdata, mid, granted_qos) :
	""" Callback for topic subscription. """
	print("Subscribed: "+str(mid)+" "+str(granted_qos))

def on_message(client, userdata, message) :
	""" Callback for when MQTT message received. """
	print("message received:\n{0}: {1}".format(message.topic, message.payload))

def on_publish(client, userdata, mid) :
	""" Callback for when MQTT message is published. """
	print("mid: {0}".format(str(mid)))

def on_disconnect(client, userdata, rc) :
	"""" Callback for MQTT broker disconnection. """
	print("Disconnect returned:")
	print("client: {0}".format(str(client)))
	print("userdata: {0}".format(str(userdata)))
	print("result: {0}".format(str(rc)))

def on_log(client, userdata, level, buf) :
	""" Callback for MQTT logging. """
	print("{0}".format(buf))

client               = mqtt.Client()
client.on_connect    = on_connect
client.on_subscribe  = on_subscribe
client.on_message    = on_message
client.on_publish    = on_publish
client.on_disconnect = on_disconnect
# Uncomment to enable debug messages
#client.on_log       = on_log

try :
	mqtt_username = config['mqtt_configuration']['MQTT_BROKER_USERNAME']
	try :
		mqtt_password = config['mqtt_configuration']['MQTT_BROKER_PASSWORD']
		client.username_pw_set(mqtt_username, mqtt_password);
	except NameError:
		print("{0}",format("No mqtt password set"))
		client.username_pw_set(mqtt_username);
except NameError:
	print("{0}",format("No mqtt username set"))

client.connect(
               config['mqtt_configuration']['MQTT_BROKER_IP'],
               int(config['mqtt_configuration']['MQTT_BROKER_PORT']),
               int(config['mqtt_configuration']['MQTT_BROKER_PORT_TIMEOUT']),
               )

client.loop_start()


def tidyupAndExit() :
	""" Stop program nicely """
	running = False       #Stop thread1
	# Disconnect mqtt client			mqttc.loop_stop()
	client.disconnect()
	print("Bye")
	exit(0)

def processInput() :
	""" Process input from keyboard """
	relay, state = raw_input("Enter relay and state in form <relay state>: ").split()
	try :
		int(relay)
	except ValueError :
		print("Please enter an integer for relay")
		return
	if not ( int(relay) in [1,2,3,4] ) :
		print("Please ensure relay number is correct")
		return
	try :
		int(state)
	except ValueError :
		print("Please enter an integer for state")
		return
	if not ( int(state) in [0,1] ) :
		print("Ensure relay state is correct!!")
		return
	payload = relay + "," + state
	client.publish("relayshield/control/relay", payload)
	return

# Loop continuously
while True :
	try :
		processInput()
	except KeyboardInterrupt :      #Triggered by pressing Ctrl+C
		tidyupAndExit()
