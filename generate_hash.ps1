param(
    [string]$executablePath,
    [string]$outputPath
)

$hash = (CertUtil -hashfile $executablePath SHA256)[1]
$hash = $hash.Replace(' ','')
Set-Content -Path $outputPath -Value $hash
