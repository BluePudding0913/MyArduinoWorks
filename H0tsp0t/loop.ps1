Start-Sleep -s 21600
while ($true) {
    Get-Date
    echo "This is running to keep the state of MobileHotspot. "
    echo "If possible, please refrain from removing this window."
    echo "(written by Asaumi)"
    powershell -NoProfile -ExecutionPolicy Unrestricted .\run.ps1
    Start-Sleep -s 61100
}
