# 座標
$x = 1894
$y = 20

# .NET Frameworkの宣言
[void] [System.Reflection.Assembly]::LoadWithPartialName("System.Drawing") 
[void] [System.Reflection.Assembly]::LoadWithPartialName("System.Windows.Forms") 

# Windows APIの宣言
$signature=@'
[DllImport("user32.dll",CharSet=CharSet.Auto,CallingConvention=CallingConvention.StdCall)]
public static extern void mouse_event(long dwFlags, long dx, long dy, long cButtons, long dwExtraInfo);
'@
$SendMouseClick = Add-Type -memberDefinition $signature -name "Win32MouseEventNew" -namespace Win32Functions -passThru

# マウスカーソル移動
[System.Windows.Forms.Cursor]::Position = New-Object System.Drawing.Point($x, $y)

# 左クリック
$SendMouseClick::mouse_event(0x0002, 0, 0, 0, 0);
$SendMouseClick::mouse_event(0x0004, 0, 0, 0, 0);