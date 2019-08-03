#!/bin/bash
echo -ne "\nRWC environment variables:\n$(env | grep RWC)\n\n"

echo "127.0.1.1 $(hostname -s)" >> /etc/hosts

#
# Create new USER with uid and gid on HOST
#

  # Add new USER to sudoers list
  sed -i '$ a oper ALL=(ALL) NOPASSWD: ALL' /etc/sudoers
  echo "Added  user 'oper'  to sudoers"

  # uncomment last 3 lines of .bashrc, i.e. enable bash-completion
  vim -T dumb -n -i NONE -es -c ':$-2,$s/^.//' -c ':wq' /root/.bashrc

  cp -pr /root/.bashrc /root/.config /root/.profile ${OPER_HOME}

  # save current env
  echo "export PATH=${PATH}" >> ${OPER_HOME}/.bashrc
  echo "export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}" >> ${OPER_HOME}/.bashrc

  echo "alias clion='/opt/clion/bin/clion.sh'" >> ${OPER_HOME}/.bashrc
  cp -r ${OPER_HOME}/additional/.CLion ${OPER_HOME}/
  cp -r ${OPER_HOME}/additional/.ssh ${OPER_HOME}/

# Start uEye USB camera service if it exists
#[[ -x /etc/init.d/ueyeusbdrc ]] && /etc/init.d/ueyeusbdrc start

usermod -a -G docker,ueye oper

su oper -s /bin/bash
