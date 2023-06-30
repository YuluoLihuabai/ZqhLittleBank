#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define INSTRUCTION_COUNT 320
#define PAGE_COUNT 4

//----------------------------------------------------------------------------------
// ����ָ������
void generateInstructions(int instructions[]) {
    int i, m, m_prime;
    
    // ��[0��INSTRUCTION_COUNT-1]�����ѡȡ���m
    m = rand() % INSTRUCTION_COUNT;
    
    for (i = 0; i < INSTRUCTION_COUNT; i++) {
        // ˳��ִ��һ��ָ��
        instructions[i] = m;
        m++;
        
        // ��ǰ��ַ����[0��m+1]�����ѡȡһ��ָ�ִ��
        if (rand() % 100 < 25 && m > 0) {
            m_prime = rand() % (m + 1);
            instructions[i] = m_prime;
        }
        
        // ˳��ִ��һ��ָ��
        instructions[i] = m_prime;
        m_prime++;
        
        // �ں��ַ����[m_prime + 2��INSTRUCTION_COUNT-1]�����ѡȡһ��ָ�ִ��
        if (rand() % 100 < 25 && m_prime + 2 < INSTRUCTION_COUNT) {
            m = rand() % (INSTRUCTION_COUNT - m_prime - 2) + m_prime + 2;
            instructions[i] = m;
        }
    }
}

// ��ָ������ת��Ϊҳ��ַ��
void convertToPageAddress(int instructions[], int page_addresses[], int page_count) {
    int i;
    
    for (i = 0; i < INSTRUCTION_COUNT; i++) {
        // ����ҳ��ַ
        page_addresses[i] = instructions[i] / PAGE_SIZE;
    }
}

//-----------------------------------------------------------------------------------------
typedef struct Page {
    int pageNumber;
    int frequency;
    int lastAccessTime;
} Page;

// ��ʼ��ҳ��
void initializePages(Page pages[]) {
    for (int i = 0; i < PAGE_COUNT; i++) {
        pages[i].pageNumber = -1;
        pages[i].frequency = 0;
        pages[i].lastAccessTime = 0;
    }
}

// ����ҳ���Ƿ����ڴ���
int findPage(Page pages[], int pageNumber) {
    for (int i = 0; i < PAGE_COUNT; i++) {
        if (pages[i].pageNumber == pageNumber) {
            return i;  // ����ҳ�����ڴ��е�����
        }
    }
    return -1;  // ҳ�治���ڴ���
}

// �������ٷ��ʵ�ҳ��
int findLFUPage(Page pages[]) {
    int minFrequency = pages[0].frequency;
    int lfuPageIndex = 0;

    for (int i = 1; i < PAGE_COUNT; i++) {
        if (pages[i].frequency < minFrequency) {
            minFrequency = pages[i].frequency;
            lfuPageIndex = i;
        }
    }

    return lfuPageIndex;  // �������ٷ��ʵ�ҳ�����ڴ��е�����
}
// ҳ���û��㷨: ���ٷ��ʣ�LFU��
void LFUAlgorithm(int instructions[], int referenceCount) {
    Page pages[PAGE_COUNT];
    int pageFaultCount = 0;

    initializePages(pages);

    for (int i = 0; i < referenceCount; i++) {
        int pageNumber = instructions[i];
        int pageIndex = findPage(pages, pageNumber);

        if (pageIndex == -1) {
            // ҳ�治���ڴ��У�����ҳ�����
            pageFaultCount++;

            int lfuPageIndex = findLFUPage(pages);
            pages[lfuPageIndex].pageNumber = pageNumber;
            pages[lfuPageIndex].frequency = 1;
            pages[lfuPageIndex].lastAccessTime = i;
        } else {
            // ҳ�����ڴ��У����ӷ���Ƶ��
            pages[pageIndex].frequency++;
            pages[pageIndex].lastAccessTime = i;
        }
    }

    printf("LFU ҳ���û��㷨:\n");
    printf("ҳ�����ô���");
    for (int i = 0; i < referenceCount; i++) {
        printf("%4d ", instructions[i]); 
    }
    printf("\n");
    printf("ҳ����������%4d\n", pageFaultCount);
    double  DoubleFaultCount=pageFaultCount;
    double hitRate=(1-DoubleFaultCount/320)*100;
    printf("������(����ֵ)��%.4f%%\n",hitRate);
}


int main() {
    int instructions[INSTRUCTION_COUNT];
    int page_addresses[INSTRUCTION_COUNT];
    int user_memory_pages, virtual_memory_size;
    system("color F0");
    srand(time(NULL));  // ��ʼ�������������
    
    // �����û��ڴ�������ҳ����
    printf("�������û��ڴ�������4-32����");
    scanf("%d", &user_memory_pages);
    
    // �����û����������K��
    printf("�������û����������32�ı�������");
    scanf("%d", &virtual_memory_size);
    
    // ����ָ������
    generateInstructions(instructions);
    
    // ��ָ������ת��Ϊҳ��ַ��
    convertToPageAddress(instructions, page_addresses, user_memory_pages);
    
    // ������
    printf("\nָ�����У�\n");
    for (int i = 0; i < INSTRUCTION_COUNT; i++) {
        printf("%4d ", instructions[i]);
    }
    
    printf("\nҳ��ַ����\n");
    for (int i = 0; i < INSTRUCTION_COUNT; i++) {
        printf("%4d ", page_addresses[i]);
    }
    
    printf("\n");
    int referenceCount = sizeof(instructions) / sizeof(instructions[0]);
	
    LFUAlgorithm(instructions, referenceCount);
    
    return 0;
}

