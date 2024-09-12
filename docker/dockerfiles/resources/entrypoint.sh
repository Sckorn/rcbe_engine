#!/bin/bash

echo "127.0.1.1 $(hostname -s)" >> /etc/hosts

#
# Create new USER with uid and gid on HOST
#
if ! $(grep -q '^radameon' /etc/group); then
  if ! $(groupadd --gid ${HGID} 'radameon'); then
    echo "E: Could not add group 'radameon'!"
    exit 1
  fi
  echo -e "Added group 'radameon' with gid=${HGID}"
fi

if ! $(grep -q '^radameon' /etc/passwd); then
  if ! $(useradd -m -N -u ${HUID} -g ${HGID} -s /bin/bash -k /root -d ${OPER_HOME} radameon); then
    echo "E: Could not add user 'radameon' to group 'radameon'!"
    exit 1
  fi
  echo -e "Added  user 'radameon'  with uid=${HUID}"
fi

if ! $(usermod -a -G users radameon); then
  echo "E: Could not add user 'radameon' to group 'users'!"
  exit 1
fi

# Add new USER to sudoers list
sed -i '$ a radameon ALL=(ALL) NOPASSWD: ALL' /etc/sudoers
echo "Added  user 'radameon'  to sudoers"

# uncomment last 3 lines of .bashrc, i.e. enable bash-completion
vim -T dumb -n -i NONE -es -c ':$-2,$s/^.//' -c ':wq' /root/.bashrc

cp -pr /root/.bashrc /root/.config /root/.profile ${OPER_HOME}

# save current env
echo "export PATH=${PATH}" >> ${OPER_HOME}/.bashrc
echo "export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}" >> ${OPER_HOME}/.bashrc

cp -r ${OPER_HOME}/additional/.ssh ${OPER_HOME}/

[[ -d ${OPER_HOME} ]] && chown -R ${HUID}:${HGID} ${OPER_HOME}
echo -e "Changed ownership of ${OPER_HOME} to 'radameon:radameon'\n"

export MESA_GL_VERSION_OVERRIDE=3.3

[[ -f ${OPER_HOME}/.ssh/id_rsa ]] && chmod go-rw ${OPER_HOME}/.ssh/id_rsa

chmod +x /start_storage.sh

echo "/start_storage.sh" >> ${OPER_HOME}/.bashrc

su radameon -s /bin/bash
