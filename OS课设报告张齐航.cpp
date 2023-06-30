#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define INSTRUCTION_COUNT 320
#define PAGE_COUNT 4

//----------------------------------------------------------------------------------
// 生成指令序列
void generateInstructions(int instructions[]) {
    int i, m, m_prime;
    
    // 在[0，INSTRUCTION_COUNT-1]间随机选取起点m
    m = rand() % INSTRUCTION_COUNT;
    
    for (i = 0; i < INSTRUCTION_COUNT; i++) {
        // 顺序执行一条指令
        instructions[i] = m;
        m++;
        
        // 在前地址部分[0，m+1]中随机选取一条指令并执行
        if (rand() % 100 < 25 && m > 0) {
            m_prime = rand() % (m + 1);
            instructions[i] = m_prime;
        }
        
        // 顺序执行一条指令
        instructions[i] = m_prime;
        m_prime++;
        
        // 在后地址部分[m_prime + 2，INSTRUCTION_COUNT-1]中随机选取一条指令并执行
        if (rand() % 100 < 25 && m_prime + 2 < INSTRUCTION_COUNT) {
            m = rand() % (INSTRUCTION_COUNT - m_prime - 2) + m_prime + 2;
            instructions[i] = m;
        }
    }
}

// 将指令序列转换为页地址流
void convertToPageAddress(int instructions[], int page_addresses[], int page_count) {
    int i;
    
    for (i = 0; i < INSTRUCTION_COUNT; i++) {
        // 计算页地址
        page_addresses[i] = instructions[i] / PAGE_SIZE;
    }
}

//-----------------------------------------------------------------------------------------
typedef struct Page {
    int pageNumber;
    int frequency;
    int lastAccessTime;
} Page;

// 初始化页面
void initializePages(Page pages[]) {
    for (int i = 0; i < PAGE_COUNT; i++) {
        pages[i].pageNumber = -1;
        pages[i].frequency = 0;
        pages[i].lastAccessTime = 0;
    }
}

// 查找页面是否在内存中
int findPage(Page pages[], int pageNumber) {
    for (int i = 0; i < PAGE_COUNT; i++) {
        if (pages[i].pageNumber == pageNumber) {
            return i;  // 返回页面在内存中的索引
        }
    }
    return -1;  // 页面不在内存中
}

// 查找最少访问的页面
int findLFUPage(Page pages[]) {
    int minFrequency = pages[0].frequency;
    int lfuPageIndex = 0;

    for (int i = 1; i < PAGE_COUNT; i++) {
        if (pages[i].frequency < minFrequency) {
            minFrequency = pages[i].frequency;
            lfuPageIndex = i;
        }
    }

    return lfuPageIndex;  // 返回最少访问的页面在内存中的索引
}
// 页面置换算法: 最少访问（LFU）
void LFUAlgorithm(int instructions[], int referenceCount) {
    Page pages[PAGE_COUNT];
    int pageFaultCount = 0;

    initializePages(pages);

    for (int i = 0; i < referenceCount; i++) {
        int pageNumber = instructions[i];
        int pageIndex = findPage(pages, pageNumber);

        if (pageIndex == -1) {
            // 页面不在内存中，发生页面错误
            pageFaultCount++;

            int lfuPageIndex = findLFUPage(pages);
            pages[lfuPageIndex].pageNumber = pageNumber;
            pages[lfuPageIndex].frequency = 1;
            pages[lfuPageIndex].lastAccessTime = i;
        } else {
            // 页面在内存中，增加访问频率
            pages[pageIndex].frequency++;
            pages[pageIndex].lastAccessTime = i;
        }
    }

    printf("LFU 页面置换算法:\n");
    printf("页面引用串：");
    for (int i = 0; i < referenceCount; i++) {
        printf("%4d ", instructions[i]); 
    }
    printf("\n");
    printf("页面错误次数：%4d\n", pageFaultCount);
    double  DoubleFaultCount=pageFaultCount;
    double hitRate=(1-DoubleFaultCount/320)*100;
    printf("命中率(近似值)：%.4f%%\n",hitRate);
}


int main() {
    int instructions[INSTRUCTION_COUNT];
    int page_addresses[INSTRUCTION_COUNT];
    int user_memory_pages, virtual_memory_size;
    system("color F0");
    srand(time(NULL));  // 初始化随机数生成器
    
    // 输入用户内存容量（页数）
    printf("请输入用户内存容量（4-32）：");
    scanf("%d", &user_memory_pages);
    
    // 输入用户虚存容量（K）
    printf("请输入用户虚存容量（32的倍数）：");
    scanf("%d", &virtual_memory_size);
    
    // 生成指令序列
    generateInstructions(instructions);
    
    // 将指令序列转换为页地址流
    convertToPageAddress(instructions, page_addresses, user_memory_pages);
    
    // 输出结果
    printf("\n指令序列：\n");
    for (int i = 0; i < INSTRUCTION_COUNT; i++) {
        printf("%4d ", instructions[i]);
    }
    
    printf("\n页地址流：\n");
    for (int i = 0; i < INSTRUCTION_COUNT; i++) {
        printf("%4d ", page_addresses[i]);
    }
    
    printf("\n");
    int referenceCount = sizeof(instructions) / sizeof(instructions[0]);
	
    LFUAlgorithm(instructions, referenceCount);
    
    return 0;
}

