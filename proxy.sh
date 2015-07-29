EXT_IP=$1
INT_IP=$2

iptables -t nat -A OUTPUT -d $EXT_IP -j DNAT --to-destination $INT_IP
iptables -t nat -A PREROUTING -d $EXT_IP -j DNAT --to-destination $INT_IP
iptables -t nat -A POSTROUTING -s $INT_IP -j SNAT --to-source $EXT_IP 

ip route add $EXT_IP dev virbr1
echo 1 >  /proc/sys/net/ipv4/conf/eth3/proxy_arp

iptables -I FORWARD -d $INT_IP -j ACCEPT
