# Get the current directory
$currentDirectory = (Get-Location).Path

# Set the host.exe path
$hostExePath = Join-Path -Path $currentDirectory -ChildPath "host.exe"

# Create the JSON object
$jsonObject = @{
    name = "dev.flandia.typstfmt_ext_host"
    description = "TypeScript Formatter Extension Host"
    path = $hostExePath
    type = "stdio"
    allowed_origins = @("chrome-extension://ebkbgnmkkebikgiohnipickpecmcilep/")
}

# Convert the JSON object to a JSON string
$jsonString = $jsonObject | ConvertTo-Json -Depth 4

# Write the JSON string to the manifest.json file
$jsonString | Set-Content -Path (Join-Path -Path $currentDirectory -ChildPath "manifest.json")

# Set the registry key and value
$registryKeyPath = "HKCU:\Software\Google\Chrome\NativeMessagingHosts\dev.flandia.typstfmt_ext_host"
$manifestJsonPath = Join-Path -Path $currentDirectory -ChildPath "manifest.json"

# Create the registry key if it doesn't exist
if (-not(Test-Path $registryKeyPath)) {
    New-Item -Path $registryKeyPath -Force
}

# Set the registry value
Set-ItemProperty -Path $registryKeyPath -Name "(Default)" -Value $manifestJsonPath
