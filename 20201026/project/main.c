#include "my.h"

int main(int argc, char const *argv[])
{
    /*************** 变量定义 ***************/
    int pos_x, pos_y;
    /*************** 打开设备 ***************/
    open_lcd();
    open_ts();

    /*************** 获取列表 ***************/
    ebm_pt pic_list = get_pic_list();
    ebm_list_show(pic_list);
    /*************** 列表排序 ***************/
    rearrange(1, "time", pic_list);
    ebm_list_show(pic_list);
    /************* 程序逻辑代码 *************/
    ebm_pt get_node;
    get_node = list_entry(&pic_list->list, ebm_st, list);
    if (!list_empty(&pic_list->list))
    {
        // 显示第一张图片
        get_node = list_entry(get_node->list.next, ebm_st, list);
        show_pic_full(get_node->path_name);
        // 循环触发
        while (1)
        {
            get_xy(&pos_x, &pos_y); // 获取xy坐标
            // get_xy_slide();
            if (pos_x >= 0 && pos_x < 400)
            {
                if (list_entry(get_node->list.prev, ebm_st, list) == pic_list)
                    get_node = list_entry(get_node->list.prev, ebm_st, list);
                get_node = list_entry(get_node->list.prev, ebm_st, list);
                show_pic_full(get_node->path_name);
            }
            else if (pos_x >= 400 && pos_x <= SIZE_W)
            {
                if (list_entry(get_node->list.next, ebm_st, list) == pic_list)
                    get_node = list_entry(get_node->list.next, ebm_st, list);
                get_node = list_entry(get_node->list.next, ebm_st, list);
                show_pic_full(get_node->path_name);
            }
        }
    }

    /*************** 关闭设备 ***************/
    close_ts();
    close_lcd();

    /*************** 释放堆空间 ***************/

    return 0;
}
