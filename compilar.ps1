# Script para compilar libfinanzas.so y ejecutar las pruebas desde Windows PowerShell usando Docker
$NativePath = Join-Path $PSScriptRoot "native"

Write-Host "Compilando libfinanzas.so y ejecutando pruebas en contenedor Linux (gcc:13-bookworm)..." -ForegroundColor Cyan
docker run --rm -v "${NativePath}:/work" -w /work gcc:13-bookworm make test
