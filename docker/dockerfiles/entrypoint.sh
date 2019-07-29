#!/bin/bash
echo -ne "\nRWC environment variables:\n$(env | grep RWC)\n\n"

echo "127.0.1.1 $(hostname -s)" >> /etc/hosts

#
# Create new USER with uid and gid on HOST
#
if ! $(grep -q '^abagy' /etc/group); then
  if ! $(groupadd --gid ${HGID} 'abagy' > /dev/null 2>&1); then
    echo "E: Could not add group 'abagy'!"
    exit 1
  fi
  echo -e "Added group 'abagy' with gid=${HGID}"
fi

if ! $(grep -q '^oper' /etc/passwd); then
  if ! $(useradd -m -N -u ${HUID} -g ${HGID} -s /bin/bash -k /root -d ${OPER_HOME} oper > /dev/null 2>&1); then
    echo "E: Could not add user 'oper' to group 'abagy'!"
    exit 1
  fi
  echo -e "Added  user 'oper'  with uid=${HUID}"

  if ! $(usermod -a -G users oper); then
    echo "E: Could not add user 'oper' to group 'users'!"
    exit 1
  fi

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

  [[ -d ${OPER_HOME} ]] && chown -R ${HUID}:${HGID} ${OPER_HOME}
  echo -e "Changed ownership of ${OPER_HOME} to 'oper:abagy'\n"

  sudo chown -R ${HUID}:${HGID} ${CATKIN_WS}
fi

[[ -f ${OPER_HOME}/.ssh/id_rsa ]] && chmod go-rw ${OPER_HOME}/.ssh/id_rsa

[[ -d ${CATKIN_WS}/src ]] && rm -r ${CATKIN_WS}/src

[[ -d ${RWC_WS}/ros ]] && ln -s ${RWC_WS}/ros/packages ${CATKIN_WS}/src && cd ${RWC_WS}

# Start uEye USB camera service if it exists
#[[ -x /etc/init.d/ueyeusbdrc ]] && /etc/init.d/ueyeusbdrc start

usermod -a -G docker,ueye oper

su oper -s /bin/bash
