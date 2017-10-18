define(`SYSTEMD_UNIT',`
[Unit]
SourcePath=$1/st.cmd
Description=Inventory Service for ICS Lab
After=network.target

[Service]
ExecStart="$1/bin/linux-x86_64/xenon1900 $1/iocBoot/iocxenon1900/st.cmd.sd"

Type=forking

[Install]
WantedBy=multi-user.target
Alias=inv.service
')
