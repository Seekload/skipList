#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MAX_LEVEL 10 //最大层数


//节点
typedef struct nodeStructure {
    int key;
    int value;
    struct nodeStructure *forward[0];
} nodeStructure;

//跳表
typedef struct skiplist {
    int level;
    nodeStructure *header;
} skiplist;

//创建节点
nodeStructure *createNode(int level, int key, int value) {
    nodeStructure *ns = (nodeStructure *) malloc(sizeof(nodeStructure) + level * sizeof(nodeStructure *));
    ns->key = key;
    ns->value = value;
    return ns;
}

//初始化跳表
skiplist *createSkiplist() {
    skiplist *sl = (skiplist *) malloc(sizeof(skiplist));
    sl->level = 0;
    sl->header = createNode(MAX_LEVEL, 0, 0);
    for (int i = 0; i < MAX_LEVEL; i++) {
        sl->header->forward[i] = NULL;
    }
    return sl;
}

//随机产生层数
int randomLevel() {
    int k = 1;
    while (rand() % 2)
        k++;
    k = (k < MAX_LEVEL) ? k : MAX_LEVEL;
    return k;
}

//插入节点
/**
 * 1、从无到有;
 * 2、插入一个层级更高的元素;
 * 3、插入一个已存在的元素;
 * 4、插入一个层级更低的元素;
 */
int insert(skiplist *sl, int key, int value) {
    nodeStructure *update[MAX_LEVEL];
    nodeStructure *p, *q = NULL;
    p = sl->header;
    int k = sl->level;
    //从最高层往下查找需要插入的位置
    //填充update
    for (int i = k - 1; i >= 0; i--) {
        //q=p->next[i]有两个作用，一是把下一节点赋值给q,二是判断下一节点是否为空
        //q 到最后总是指向 终止的元素或者null
        while ((q = p->forward[i]) && (q->key < key)) {
            p = q;
        }

        update[i] = p;
    }
    //不能插入相同的key
    if (q && q->key == key) {
        return 0;
    }

    //产生一个随机层数K
    //新建一个待插入节点q
    //一层一层插入
    k = randomLevel();
    //更新跳表的level
    if (k > (sl->level)) {
        for (int i = sl->level; i < k; i++) {
            update[i] = sl->header;
        }
        sl->level = k;
    }

    q = createNode(k, key, value);
    //逐层更新节点的指针，和普通列表插入一样
    for (int i = 0; i < k; i++) {
        q->forward[i] = update[i]->forward[i];
        update[i]->forward[i] = q;
    }
    return 1;
}

//搜索指定key的value
int search(skiplist *sl, int key) {
    nodeStructure *p, *q = NULL;
    p = sl->header;
    //从最高层开始搜
    int k = sl->level;
    for (int i = k - 1; i >= 0; i--) {
        while ((q = p->forward[i]) && (q->key <= key)) {
            if (q->key == key) {
                return q->value;
            }
            p = q;
        }
    }
    return NULL;
}

//删除指定的key
int deleteSL(skiplist *sl, int key) {
    nodeStructure *update[MAX_LEVEL];
    nodeStructure *p, *q = NULL;
    p = sl->header;
    //从最高层开始搜
    int k = sl->level;

    //q 到最后总是指向  要删除的元素
    for (int i = k - 1; i >= 0; i--) {
        while ((q = p->forward[i]) && (q->key < key)) {
            p = q;
        }
        //update[i] 记录要删除节点的forward[i]的"前驱"节点
        update[i] = p;
    }
    if (q && q->key == key) {
        //逐层删除，和普通列表删除一样
        for (int i = 0; i < sl->level; i++) {
            if (update[i]->forward[i] == q) {
                update[i]->forward[i] = q->forward[i];
            }
        }
        free(q);
        //如果删除的是最大层的节点，那么需要重新维护跳表的层数
        for (int i = sl->level - 1; i >= 0; i--) {
            if (sl->header->forward[i] == NULL) {
                sl->level--;
            }
        }
        return 1;
    }
    else
        return 0;
}

void printSL(skiplist *sl) {
    //从最高层开始打印
    nodeStructure *p, *q = NULL;

    //从最高层开始搜
    int k = sl->level;
    for (int i = k - 1; i >= 0; i--) {
        p = sl->header;
        while (q = p->forward[i]) {
            printf("%d -> ", p->value);
            p = q;
        }
        printf("\n");
    }
    printf("\n");
}

int main() {

    skiplist *sl = createSkiplist();
    for (int i = 1; i <= 19; i++) {
        insert(sl, i, i * 2);
    }
    printSL(sl);
    //搜索
    int i = search(sl, 4);
    printf("i=%d\n", i);
    //删除
    int b = deleteSL(sl, 4);
    if (b)
        printf("删除成功\n");
    printSL(sl);
    system("pause");
    return 0;
}


