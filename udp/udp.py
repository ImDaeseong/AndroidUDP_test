import socket
import time


def UdpSend():
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    sock.setsockopt(socket.SOL_SOCKET, socket.SO_BROADCAST, True)

    i = 0
    while True:
        sock.sendto(b'(%d) test message' % i, ('255.255.255.255', 20121))
        i += 1
        time.sleep(1)


def Listen():
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, True)
    sock.setsockopt(socket.SOL_SOCKET, socket.SO_BROADCAST, True)
    sock.bind(('', 20121))

    while True:
        data, address = sock.recvfrom(1024)
        text = data.decode('ascii')
        # print(text)
        # print(address)
        # print('주소:{} 메시지:{!r}'.format(address, text))
        sock.sendto(b'return test message', address)


if __name__ == '__main__':
    # UdpSend()
    Listen()

