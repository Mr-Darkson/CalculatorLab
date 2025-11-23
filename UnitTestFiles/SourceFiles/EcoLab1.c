/*
 * <кодировка символов>
 *   Cyrillic (UTF-8 with signature) - Codepage 65001
 * </кодировка символов>
 *
 * <сводка>
 *   EcoLab1
 * </сводка>
 *
 * <описание>
 *   Данный исходный файл является точкой входа
 * </описание>
 *
 * <автор>
 *   Copyright (c) 2018 Vladimir Bashev. All rights reserved.
 * </автор>
 *
 */


/* Eco OS */
#include "IEcoSystem1.h"
#include "IdEcoMemoryManager1.h"
#include "IdEcoInterfaceBus1.h"
#include "IdEcoFileSystemManagement1.h"
#include "IdEcoLab1.h"
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <stdio.h>

#include "IEcoCalculatorX.h"
#include "IEcoCalculatorY.h"
#include "IdEcoCalculatorA.h"
#include "IdEcoCalculatorB.h"
#include "IdEcoCalculatorD.h"
#include "IdEcoCalculatorE.h"

/*
 *
 * <сводка>
 *   Функция EcoMain
 * </сводка>++
 *
 * <описание>
 *   Функция EcoMain - точка входа
 * </описание>
 *
 */
int16_t EcoMain(IEcoUnknown* pIUnk) {
    int16_t result = -1;
    /* Указатель на системный интерфейс */
    IEcoSystem1* pISys = 0;
    /* Указатель на интерфейс работы с системной интерфейсной шиной */
    IEcoInterfaceBus1* pIBus = 0;
    /* Указатель на интерфейс работы с памятью */
    IEcoMemoryAllocator1* pIMem = 0;
    char_t* name = 0;
    char_t* copyName = 0;
    /* Указатель на тестируемый интерфейс */
    IEcoLab1* pIEcoLab1 = 0;

    int32_t addResult;
    int16_t subResult;
    
    /* Объявления для тестирования сортировки */
    int64_t test_array[] = {64, 34, 25, 12, 22, 11, 90, 5};
    uint32_t array_size = sizeof(test_array) / sizeof(test_array[0]);
    uint32_t i;  /* Объявляем переменную цикла в начале */

    /* Проверка и создание системного интерфейса */
    if (pISys == 0) {
        result = pIUnk->pVTbl->QueryInterface(pIUnk, &GID_IEcoSystem, (void **)&pISys);
        if (result != 0 && pISys == 0) {
            goto Release;
        }
    }

    /* Получение интерфейса для работы с интерфейсной шиной */
    result = pISys->pVTbl->QueryInterface(pISys, &IID_IEcoInterfaceBus1, (void **)&pIBus);
    if (result != 0 || pIBus == 0) {
        goto Release;
    }

#ifdef ECO_LIB
    /* Регистрация статического компонента для работы со списком */
    result = pIBus->pVTbl->RegisterComponent(pIBus, &CID_EcoLab1, (IEcoUnknown*)GetIEcoComponentFactoryPtr_1F5DF16EE1BF43B999A434ED38FE8F3A);
    if (result != 0 ) {
        goto Release;
    }
#endif

    /* Получение интерфейса управления памятью */
    result = pIBus->pVTbl->QueryComponent(pIBus, &CID_EcoMemoryManager1, 0, &IID_IEcoMemoryAllocator1, (void**) &pIMem);
    if (result != 0 || pIMem == 0) {
        goto Release;
    }
    
    /* Получение тестируемого интерфейса */
    result = pIBus->pVTbl->QueryComponent(pIBus, &CID_EcoLab1, 0, &IID_IEcoLab1, (void**) &pIEcoLab1);
    if (result != 0 || pIEcoLab1 == 0) {
        printf("Failed to get IEcoLab1 interface\n");
        goto Release;
    }

	printf("\n=== Calculator Interfaces Demonstration ===\n\n");

    /* IEcoCalculatorX напрямую из IEcoLab1 */
    if (pIEcoLab1->pVTbl->QueryInterface(pIEcoLab1, &IID_IEcoCalculatorX, (void**)&pIX) == 0 && pIX) {
        printf("IEcoCalculatorX obtained directly from IEcoLab1\n");
        printf(" 1) 1337 + 812 = %d\n", pIX->pVTbl->Addition(pIX, 1337, 812));
        printf(" 2) 321 - 123 = %d\n", pIX->pVTbl->Subtraction(pIX, 321, 123));

        /* IEcoUnknown через IEcoCalculatorX */
        if (pIX->pVTbl->QueryInterface(pIX, &IID_IEcoUnknown, (void**)&pOtherIUnknown) == 0 && pOtherIUnknown) {
            printf("IEcoUnknown obtained from IEcoCalculatorX\n");
            pOtherIUnknown->pVTbl->Release(pOtherIUnknown);
        } else {
            printf("Error: Failed to get IEcoUnknown from IEcoCalculatorX\n");
        }

        /* IEcoCalculatorY через IEcoCalculatorX */
        if (pIX->pVTbl->QueryInterface(pIX, &IID_IEcoCalculatorY, (void**)&pOtherIY) == 0 && pOtherIY) {
            printf("IEcoCalculatorY obtained from IEcoCalculatorX\n");
            printf(" 1) 52 * 42 = %d\n", pOtherIY->pVTbl->Multiplication(pOtherIY, 52, 42));
            printf(" 2) 333 / 33 = %d\n", pOtherIY->pVTbl->Division(pOtherIY, 333, 33));
            pOtherIY->pVTbl->Release(pOtherIY);
        } else {
            printf("Error: Failed to get IEcoCalculatorY from IEcoCalculatorX\n");
        }

        /* IEcoLab1 через IEcoCalculatorX */
        if (pIX->pVTbl->QueryInterface(pIX, &IID_IEcoLab1, (void**)&pOtherIEcoLab1) == 0 && pOtherIEcoLab1) {
            printf("IEcoLab1 obtained from IEcoCalculatorX\n");
            pOtherIEcoLab1->pVTbl->Release(pOtherIEcoLab1);
        } else {
            printf("Error: Failed to get IEcoLab1 from IEcoCalculatorX\n");
        }

        pIX->pVTbl->Release(pIX);
    }
    else {
        printf("Error: Failed to get IEcoCalculatorX from IEcoLab1\n");
    }
    printf("\n");

    /* IEcoCalculatorY напрямую из IEcoLab1 */
    if (pIEcoLab1->pVTbl->QueryInterface(pIEcoLab1, &IID_IEcoCalculatorY, (void**)&pIY) == 0 && pIY) {
        printf("IEcoCalculatorY obtained directly from IEcoLab1\n");
        printf(" 1) 13 * 100 = %d\n", pIY->pVTbl->Multiplication(pIY, 13, 100));
        printf(" 2) 99 / 11 = %d\n", pIY->pVTbl->Division(pIY, 99, 11));

        /* IEcoUnknown через IEcoCalculatorY */
        if (pIY->pVTbl->QueryInterface(pIY, &IID_IEcoUnknown, (void**)&pOtherIUnknown) == 0 && pOtherIUnknown) {
            printf("IEcoUnknown obtained from IEcoCalculatorY\n");
            pOtherIUnknown->pVTbl->Release(pOtherIUnknown);
        } else {
            printf("Error: Failed to get IEcoUnknown from IEcoCalculatorY\n");
        }

        /* IEcoCalculatorX через IEcoCalculatorY */
        if (pIY->pVTbl->QueryInterface(pIY, &IID_IEcoCalculatorX, (void**)&pOtherIX) == 0 && pOtherIX) {
            printf("IEcoCalculatorX obtained from IEcoCalculatorY\n");
            printf(" 1) 555 + 777 = %d\n", pOtherIX->pVTbl->Addition(pOtherIX, 555, 777));
            printf(" 2) 450 - 50 = %d\n", pOtherIX->pVTbl->Subtraction(pOtherIX, 450, 50));
            pOtherIX->pVTbl->Release(pOtherIX);
        } else {
            printf("Error: Failed to get IEcoCalculatorX from IEcoCalculatorY\n");
        }

        /* IEcoLab1 через IEcoCalculatorY */
        if (pIY->pVTbl->QueryInterface(pIY, &IID_IEcoLab1, (void**)&pOtherIEcoLab1) == 0 && pOtherIEcoLab1) {
            printf("IEcoLab1 obtained from IEcoCalculatorY\n");
            pOtherIEcoLab1->pVTbl->Release(pOtherIEcoLab1);
        } else {
            printf("Error: Failed to get IEcoLab1 from IEcoCalculatorY\n");
        }

        pIY->pVTbl->Release(pIY);
    }
    else {
        printf("Error: Failed to get IEcoCalculatorY from IEcoLab1\n");
    }
	 printf("\n");

	/* Получаем IEcoUnknown из IEcoLab1 */
    if (pIEcoLab1->pVTbl->QueryInterface(pIEcoLab1, &IID_IEcoUnknown, (void**)&pIUnknown) == 0 && pIUnknown) {
        printf("IEcoUnknown obtained from IEcoLab1\n");

        /* IEcoLab1 через IEcoUnknown */
        if (pIUnknown->pVTbl->QueryInterface(pIUnknown, &IID_IEcoLab1, (void**)&pOtherIEcoLab1) == 0 && pOtherIEcoLab1) {
            printf("IEcoLab1 obtained from IEcoUnknown\n");
            pOtherIEcoLab1->pVTbl->Release(pOtherIEcoLab1);
        } else {
            printf("Error: Failed to get IEcoLab1 from IEcoUnknown\n");
        }

        /* IEcoCalculatorX через IEcoUnknown */
        if (pIUnknown->pVTbl->QueryInterface(pIUnknown, &IID_IEcoCalculatorX, (void**)&pOtherIX) == 0 && pOtherIX) {
            printf("IEcoCalculatorX obtained from IEcoUnknown\n");
            printf(" 1) 44 + 66 = %d\n", pOtherIX->pVTbl->Addition(pOtherIX, 44, 66));
            printf(" 2) 999 - 9 = %d\n", pOtherIX->pVTbl->Subtraction(pOtherIX, 999, 9));
            pOtherIX->pVTbl->Release(pOtherIX);
        } else {
            printf("Error: Failed to get IEcoCalculatorX from IEcoUnknown\n");
        }

        /* IEcoCalculatorY через IEcoUnknown */
        if (pIUnknown->pVTbl->QueryInterface(pIUnknown, &IID_IEcoCalculatorY, (void**)&pOtherIY) == 0 && pOtherIY) {
            printf("IEcoCalculatorY obtained from IEcoUnknown\n");
            printf(" 1) 10 * 100 = %d\n", pOtherIY->pVTbl->Multiplication(pOtherIY, 10, 100));
            printf(" 2) 1000 / 5 = %d\n", pOtherIY->pVTbl->Division(pOtherIY, 1000, 5));
            pOtherIY->pVTbl->Release(pOtherIY);
        } else {
            printf("Error: Failed to get IEcoCalculatorY from IEcoUnknown\n");
        }

        pIUnknown->pVTbl->Release(pIUnknown);
    } else {
        printf("Error Failed to get IEcoUnknown from IEcoLab1\n");
    }

    printf("\n=== End of Interface Demonstration ===\n\n");

    printf("=== ТЕСТИРОВАНИЕ КОМПОНЕНТА CEcoLab1 ===\n\n");

    /* Тестирование функции MyFunction */
    name = (char_t *)pIMem->pVTbl->Alloc(pIMem, 10);
    pIMem->pVTbl->Fill(pIMem, name, 'a', 9);
    name[9] = '\0';  /* Ensure null termination */
    
    result = pIEcoLab1->pVTbl->MyFunction(pIEcoLab1, name, &copyName);
    if (result == ERR_ECO_SUCCESES && copyName != 0) {
        printf("MyFunction test passed. Original: '%s', Copy: '%s'\n", name, copyName);
    } else {
        printf("MyFunction test failed\n");
    }

    /* Тестирование сортировки перемешиванием */
    
    /* Вывод массива до сортировки */
    printf("Array before sorting: ");
    for (i = 0; i < array_size; i++) {
        printf("%lld ", test_array[i]);
    }
    printf("\n");
    
    /* Вызов функции сортировки */
    result = pIEcoLab1->pVTbl->cocktail_sort_int64(pIEcoLab1, test_array, array_size);
    if (result != ERR_ECO_SUCCESES) {
        printf("Sorting error\n");
        goto Release;
    }
    
    /* Вывод массива после сортировки */
    printf("Array after sorting: ");
    for (i = 0; i < array_size; i++) {
        printf("%lld ", test_array[i]);
    }
    printf("\n");

    printf("\n=== All tests completed successfully ===\n");

Release:
    
    /* Освобождение блока памяти */
    if (name != 0) {
        pIMem->pVTbl->Free(pIMem, name);
    }
    
    /* Освобождение остальных интерфейсов */
    if (pIBus != 0) {
        pIBus->pVTbl->Release(pIBus);
    }
    if (pIMem != 0) {
        pIMem->pVTbl->Release(pIMem);
    }
    if (pIEcoLab1 != 0) {
        pIEcoLab1->pVTbl->Release(pIEcoLab1);
    }
    if (pISys != 0) {
        pISys->pVTbl->Release(pISys);
    }
    
    return result;
}