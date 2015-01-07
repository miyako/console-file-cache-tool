# console-file-cache-tool
A Windows console program to raed and write the system file cache size limits.

About
-----
There is a [known issue on Windows earlier than Server 2008 R2 SP1](http://support.microsoft.com/kb/976618), where the system file cache could consume so much physical memory that other running applications could experience severe cosequences.

The problem is especially serious on 32 bit systems, since adding more memory to the system will not improve the situation; all available memory addresses will be taken by the system file cache.

For example, you may not be able to complete a conversion of a large 4D database.

One way to solve this issue is to use [SetSystemFileCacheSize](http://msdn.microsoft.com/en-us/library/aa965240(VS.85).aspx) and impose a limit on the amount of memory the system file cache can consume.

This console program is designed to let you do just that from the command-line.

**Note**
The problem should be solved as of Windows Server 2008 R2 SP1. In addition, there is a [fix] (http://support.microsoft.com/kb/979149) for Windows 7 and Windows 2008 R2.

This program is mainly intended for users running a large database application on systems before Windows Server 2008 R2, and having the kind of problems described above.

**関連記事**
http://blogs.technet.com/b/askcorejp/archive/2009/12/29/1-64bit.aspx

Example
-------
コマンドプロンプト（windows/system32/cmd.exe）に対するショートカットをデスクトップなどに作成します。

作成したショートカットを右クリックし，『管理者として実行』します。

コマンドプロンプトより，file_cache_toolを実行します。

**オプション**
-m 最大値を設定する（ある程度のサイズでないと87エラーが返されます。）
-n 最小値を設定する（0はデフォルト値になります。）
-i 現在の値を確認することができます
-l システムファイルキャッシュのフラッシュを実行します

![](images/1.png)

