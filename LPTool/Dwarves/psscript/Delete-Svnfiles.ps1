
process
{
    get-childitem -R -force $_ | Where-Object { $_.Name.Contains('svn') } | remove-item -r -force
}

