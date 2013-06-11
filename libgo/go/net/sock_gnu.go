// copied from 2d2d80b8bd963f59534897b3d51ef8bd546cb4bc:libgo/go/net/sock_bsd.go

// Copyright 2009 The Go Authors.  All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

// Sockets for GNU/Hurd

package net

import "syscall"

func maxListenerBacklog() int {
       // TODO: Implement this
       return syscall.SOMAXCONN
}

func listenerSockaddr(s, f int, la syscall.Sockaddr, toAddr func(syscall.Sockaddr) Addr) (syscall.Sockaddr, error) {
	a := toAddr(la)
	if a == nil {
		return la, nil
	}
	switch v := a.(type) {
	case *TCPAddr, *UnixAddr:
		err := setDefaultListenerSockopts(s)
		if err != nil {
			return nil, err
		}
	case *UDPAddr:
		if v.IP.IsMulticast() {
			err := setDefaultMulticastSockopts(s)
			if err != nil {
				return nil, err
			}
			switch f {
			case syscall.AF_INET:
				v.IP = IPv4zero
			case syscall.AF_INET6:
				v.IP = IPv6unspecified
			}
			return v.sockaddr(f)
		}
	}
	return la, nil
}
