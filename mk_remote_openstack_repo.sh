for i in ceilometer heat neutron-fwaas python2-lxc python-heatclient python-openstackclient tempest cinder horizon neutron-lbaas \
        python-ceilometerclient python-keystoneclient python-swiftclient glance keystone neutron-vpnaas python-cinderclient \
        python-neutronclient requirements glance_store neutron nova python-glanceclient python-novaclient swift; do
     git clone --bare https://github.com/openstack/$i $i
     cd $i
     git --bare update-server-info
     cd -
done
