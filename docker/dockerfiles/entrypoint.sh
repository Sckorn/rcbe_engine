#!/bin/bash

echo "127.0.1.1 $(hostname -s)" >> /etc/hosts

#
# Create new USER with uid and gid on HOST
#
if ! $(grep -q '^rcbe' /etc/group); then
  if ! $(groupadd --gid ${HGID} 'rcbe'); then
    echo "E: Could not add group 'rcbe'!"
    exit 1
  fi
  echo -e "Added group 'rcbe' with gid=${HGID}"
fi

if ! $(grep -q '^oper' /etc/passwd); then
  if ! $(useradd -m -N -u ${HUID} -g ${HGID} -s /bin/bash -k /root -d ${OPER_HOME} oper); then
    echo "E: Could not add user 'oper' to group 'rcbe'!"
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
  echo -e "Changed ownership of ${OPER_HOME} to 'oper:rcbe'\n"

fi

export MESA_GL_VERSION_OVERRIDE=3.3

[[ -f ${OPER_HOME}/.ssh/id_rsa ]] && chmod go-rw ${OPER_HOME}/.ssh/id_rsa

chmod +x /start_storage.sh

echo "/start_storage.sh" >> ${OPER_HOME}/.bashrc

su oper -s /bin/bash
