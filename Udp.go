// Udp
package main

import (
	"fmt"
	"log"
	"net"
	"time"
)

func SendData(data string) {

	addr, err := net.ResolveUDPAddr("udp", "255.255.255.255:20121")
	if err != nil {
		log.Panic(err)
	}

	conn, err := net.DialUDP("udp", nil, addr)
	defer conn.Close()

	if err != nil {
		log.Panic(err)
	}

	_, err = conn.Write([]byte(data))

	if err != nil {
		log.Panic(err)
	}

}

func ReceiveData() {

	addr, err := net.ResolveUDPAddr("udp", ":20121")
	if err != nil {
		log.Panic(err)
	}

	conn, err := net.ListenUDP("udp", addr)
	defer conn.Close()

	if err != nil {
		log.Panic(err)
	}

	var buf [1024]byte
	for {
		n, raddr, err := conn.ReadFromUDP(buf[0:])
		if err != nil {
			return
		}

		sMsg := fmt.Sprintf("%s %s", string(buf[0:n]), raddr)
		fmt.Println(sMsg)

		returnMsg := []byte("return test message")
		_, err = conn.WriteToUDP(returnMsg, raddr)

		if err != nil {
			log.Panic(err)
		}
	}
}

func UdpListen() {

	Ticker := time.NewTicker(time.Millisecond * 1000)
	defer Ticker.Stop()

	for {
		select {
		case <-Ticker.C:
			ReceiveData()
		}
	}

}

func UdpSend() {

	Ticker := time.NewTicker(time.Millisecond * 1000)
	defer Ticker.Stop()

	for {
		select {
		case <-Ticker.C:
			SendData("test message test message test message test message test message")
		}
	}

}

func main() {

	UdpSend()

	//UdpListen()

}
