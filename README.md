# <p align="center">EMLPROJ</p>
================================
文件结构：
>antivirus 【反病毒和反垃圾模块】
>>spam 反垃圾模块，调用垃圾库</br> 
>>virus 反病毒模块，调用病毒库
>
>appendix 【附件处理模块】
>>jpeg jpeg图像文件处理，主要提取图片extra信息，调用exif库实现</br> 
>>pdf PDF文本文件提取和附加信息提取</br> 
>>zips 解压缩文件提取和处理</br> 
>
>gmime 邮件格式解析和附件提取，依赖gmime2.6.11实现
>
>main 主函数入口部分
>>bc 临时文件</br> 
>>Data 系统测试依赖库</br> 
>>temps 程序运行临时文件夹</br>
>>testdata 测试输入文件集</br>
>>testfile 附件临时提取内容存放文件夹</br>
>
>regex 正则解析模块，提取关键字类，依赖pcre系统库
>
>spliter 中文分词模块，处理关键字，依赖ICTCLAS3.0


<p align="right">NEWPLAN </br>2016.3.11</p>


