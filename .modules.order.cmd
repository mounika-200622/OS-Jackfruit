cmd_/home/sharvari_naik/os-jack/modules.order := {   echo /home/sharvari_naik/os-jack/monitor.ko; :; } | awk '!x[$$0]++' - > /home/sharvari_naik/os-jack/modules.order
