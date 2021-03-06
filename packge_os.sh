#os package
dst_iso_name=ubuntu.iso
src_iso_name=ubuntu-14.04.2-server-amd64.iso

mkdir os new
mount -o loop $src_iso_name os
# cd os; find . | cpio -pd ../new; cd -
rsync -a os/ new/
umount os

cd new; find . -type f -print0 | xargs -0 md5sum | grep -v "\./md5sum.txt" | tee ./md5sum.txt; cd -

mkisofs -quiet -r -J -R -b isolinux/isolinux.bin  -no-emul-boot -boot-load-size 5 -boot-info-table -hide-rr-moved -x "lost+found:" -o $dst_iso_name ./new
#rm -rf os new


#make ubuntu repo
atp-get install reprepro
mkdir repo repo/conf
cat <<EOF > distributions
Codename: trusty
Components: main
Architectures: i386 amd64
EOF

reprepro -b dst_repo_dir includedeb trusty deb_name


#make centos repo
yum install

