"First Argument is : " + $args[0]
"Second Argument is : " + $args[1]

$file = 'D:\urlupdate.txt'
$sqlf = 'UPDATE package_info SET url = ''{0}/{1}'' WHERE pck_id = {2};'
$urldir = $args[1]

#get-childitem $args[0] | select name

get-childitem $args[0] | foreach-object `
{
    #add-content $file  $_ 
    $serial = $_.Name.split('-')[0];
    $sql = $sqlf -f $urldir,$_,$serial
    $sql
    #add-content $file $sql
}
