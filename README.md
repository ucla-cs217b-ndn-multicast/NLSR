NLSR0.0
=======

Named Data Link State Routing

Installation:

1: Getting NLSR: 

git clone https://github.com/NDN-Routing/NLSR0.0
git checkout nlsr-cpp

2: Compiling NLSR
	a. Run ./configure
	b. Run make
	c. For installing run sudo make install
	
 Prerequisites:
   i. You must have ndnx install in your machine
   
   ii. For smooth compilation you will need need autoconf-2.69 which can 
   be fethced from http://ftpmirror.gnu.org/autoconf/autoconf-2.69.tar.gz
   and automake-1.14 which can be fethed from 
   http://gnu.mirrorcatalogs.com/automake/automake-1.14.tar.gz

Running NLSR:

1: Run ndnx daemon

2: Add '/ndn/keys' route to your neighboring testbed nodes. 
This can be done by adding routes in your ccnd.conf to all neigbors,
e.g.
	add /ndn/keys udp 10.0.XX.XX 
	add /ndn/keys udp 10.0.YY.YY ...  
	add	/ndn/keys udp 10.0.ZZ.ZZ

3: Create your keys and publish them through repo. Please have
	a look at https://github.com/named-data/ndn-testbed-key-suite for details. 

4: create a nlsr configuration file, have a look at
	macbook.conf and 'Configuration Parameters' section  for details 

7: start nlsr. You can find help for running nlsr with "-h"


Configuration Parameter: 

1. router-name </name/prefix/of/the/router> 
It is a required configuration parameter that is used to set the router name.
Name prefix of the router must start with `/ndn' and followed by a strict
hierarchical order of name components up to the hostname of the router. For
e.g. if router has a hostname host.dept.org.edu pollux.cs.memphis.edu then the
router name can be `/ndn/org.edu/dept/host..

2. ccnneighbor </name/prefix/of/neighbor> [neighbor_ip_addrr] 
This parameter is used to configure the neighboring router. Name prefix of a
neighbor must start with `/ndn. and followed by a strict hierarchical order of
name component as mentioned for router-name. If a router has a neighbor having
hostname `neighbor.dept.org.edu' then configuration command would be
`ccnneighbor /ndn/org.edu/dept/neighbor. The optional parameter,
[neighbor_ip_addrr], can be provided for explicitly connecting to a given 
IP. This is useful if there is no entry of the hostname in the DNS. 

3. ccnname </name/prefix/to/be/advertised> 
ccnname is used to advertise the name prefix of the services hosted by the
router. Name prefix must follow the ccnx naming convention.

4. topo-prefix <name/prefix/of/repo/topo> 
It is used to configure the topological prefix of the `sync. slice created for
nlsr in a repo. Prefix must follow the ccnx naming convention. Default value
for topo prefix is `/ndn/routing/nlsr'

5. slice-prefix <name/prefix/of/repo/slice>
This parameter is used to set the sync's slice prefix. All nlsr nodes `must'
have the same slice prefix and must follow the ccnx naming convention. Default
value for slice prefix is `/ndn/routing/nlsr/LSA'

6. lsa-refresh-time <sec> 
It is the time in seconds, after which router will refresh its LSAs. Default
value = 1800 sec, Min = 240 sec

7. router-dead-interval <sec> 
It is the time after which router will consider its neighbor dead if it does
not hear anything for that amount of time. If an LSA generated by neighbor has
a lifetime greater than router-dead-interval then router will simply discard
the LSA and update name prefix table, routing table and ccnd FIB accordingly.
Default value = 3600 sec, Min = 360 sec.

8. interest-retry <number> 
interest-retry is maximum number of times a router will send same interest if
the interest is timed out. Default = 3, Min =1 and Max = 10

9. interest-resend-time <sec>
When an interest is timed out, NLSR waits for `interest-resend-time. before
sending another interest. The interest is retransmitted up to the value of
`interest-retry.. If the interest doesn.t get satisfied during this time the
neighbor will be considered as dead. Default value = 15 sec, Min = 1 sec, Max =
60 sec 

10. max-faces-per-prefix <number>
By default NLSR calculates and add faces for all available faces to a prefix.
Number of faces can be restricted with this parameter, if this is configured
NLSR will add <number> or faces per prefixes in ccnd FIB.

11. debug <on/off> 
Enable nlsr debugging by printing out detail messages on the standard output.
By default it is off. 

12. logdir </directory/for/logging/> 
NLSR will start writing log in logdir folder. If logdir is not configured NLSR
will create a log directory name nlsrLog in user home directory and write log
into that folder.

13. tunnel-type <udp | tcp> 
This is used to specify the tunnel type that NLSR should create for building an
overlay network. By default NLSR creates udp tunnels.

**Following Commands are only required for hyperbolic routing**

14. hyperbolic-routing <on|dry_run> 
This option enables hyperbolic routing. With option on, link state routing
doesn't work while with dry_run option route is calculated by Link State 
routing and hyperbolic routes are just recorded only in log file.Following 
field is required if hyperbolic routing is on. 

15. hyperbolic-cordinate <radius> <theta> 
Polar coordinates of a node in a form of radius and theta. Currently, calculation 
of the polar coordinates is an offline process and done by 
the operator of a site. 

** Hyperbolic Commands End here **

16. keystore-path </path/to/keystore/file name(.ccnx_keystore)>
Path to the keystore. The keystore folder is usually created  by
ccninitkeystore. Keys in this folder will be used by NLSR to sign the packets. 

17. root-key-prefix </key/prefix>   
Name prefix of the root key; the site key should be signed by this key.  
e.g. root-key-prefix /ndn/keys

18. site-name </name/prefix/of/site> 
Name of the site of which this router is a part. 

19. network </name/of/network>
Name of the networks router belongs to 
