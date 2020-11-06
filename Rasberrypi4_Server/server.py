## < Rasberry Pi -Server >
import socket
import fcntl
import struct
import asyncio
import websockets
import time
def get_ipaddress(network):
    s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    return socket.inet_ntoa(fcntl.ioctl(
        s.fileno(),
        0x8915, #SIOCGIFADDR
        struct.pack('256s',network[:24].encode('utf-8'))
        )[20:24])


async def echo(websocket, path):
    try:
        async for message in websocket :
            print(message)
            time.sleep(1)
    except:
        print(".");
        pass

        #await websocket.send(message)


print("Server start  :" + get_ipaddress('wlan0'))

port = 9999 
asyncio.get_event_loop().run_until_complete(
    websockets.serve(echo,get_ipaddress('wlan0'), port))
asyncio.get_event_loop().run_forever()

