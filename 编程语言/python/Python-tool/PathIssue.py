# 模块说明：
# 1.此模块用来解决模块导入的路径问题
# 2.统一将当前脚本的上层目录（或者上上层目录）加入搜索路径 

import os,sys
BasePathFSearch = os.path.abspath(os.path.join(sys.argv[0],'../')) #获取本层路径   
ParentPathFSearch = os.path.abspath(os.path.join(BasePathFSearch,'../'))  #获取脚本上层路径
sys.path.append(ParentPathFSearch)
GrandPathFSearch = os.path.abspath(os.path.join(BasePathFSearch,'../..'))  #获取脚本上上层路径
sys.path.append(GrandPathFSearch)

print (sys.path)