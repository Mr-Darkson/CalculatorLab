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
#include <stdio.h>

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

    printf("=== ТЕСТИРОВАНИЕ КОМПОНЕНТА CEcoLab1 ===\n\n");

    /* ТЕСТИРОВАНИЕ КАЛЬКУЛЯТОРА */
    printf("--- Calculator test ---\n");
    
    /* Тест сложения */
    addResult = pIEcoLab1->pVTbl->CalcAdd(pIEcoLab1, 15, 7);
    printf("Addition: 15 + 7 = %d\n", addResult);
    
    addResult = pIEcoLab1->pVTbl->CalcAdd(pIEcoLab1, -5, 10);
    printf("Addition: -5 + 10 = %d\n", addResult);
    
    addResult = pIEcoLab1->pVTbl->CalcAdd(pIEcoLab1, 100, 200);
    printf("Addition: 100 + 200 = %d\n", addResult);
    
    /* Тест вычитания */
    subResult = pIEcoLab1->pVTbl->CalcSubtract(pIEcoLab1, 20, 8);
    printf("Subtraction: 20 - 8 = %d\n", subResult);
    
    subResult = pIEcoLab1->pVTbl->CalcSubtract(pIEcoLab1, 5, 12);
    printf("Subtraction: 5 - 12 = %d\n", subResult);
    
    subResult = pIEcoLab1->pVTbl->CalcSubtract(pIEcoLab1, 1000, 500);
    printf("Subtraction: 1000 - 500 = %d\n", subResult);
    
    /* Комплексный тест */
    printf("\nComplex test:\n");
    addResult = pIEcoLab1->pVTbl->CalcAdd(pIEcoLab1, 50, 25);
    subResult = pIEcoLab1->pVTbl->CalcSubtract(pIEcoLab1, addResult, 10);
    printf("(50 + 25) - 10 = %d\n", subResult);

     printf("--- Calculator test finished ---\n\n");


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