/* AUTOGENERATED FILE. DO NOT EDIT. */
#ifndef _MOCKTASK_H
#define _MOCKTASK_H

#include "unity.h"
#include "task.h"
#include "CException.h"

/* Ignore the following warnings, since we are copying code */
#if defined(__GNUC__) && !defined(__ICC) && !defined(__TMS470__)
#if __GNUC__ > 4 || (__GNUC__ == 4 && (__GNUC_MINOR__ > 6 || (__GNUC_MINOR__ == 6 && __GNUC_PATCHLEVEL__ > 0)))
#pragma GCC diagnostic push
#endif
#if !defined(__clang__)
#pragma GCC diagnostic ignored "-Wpragmas"
#endif
#pragma GCC diagnostic ignored "-Wunknown-pragmas"
#pragma GCC diagnostic ignored "-Wduplicate-decl-specifier"
#endif

void Mocktask_Init(void);
void Mocktask_Destroy(void);
void Mocktask_Verify(void);




#define taskENTER_CRITICAL_Ignore() taskENTER_CRITICAL_CMockIgnore()
void taskENTER_CRITICAL_CMockIgnore(void);
#define taskENTER_CRITICAL_Expect() taskENTER_CRITICAL_CMockExpect(__LINE__)
void taskENTER_CRITICAL_CMockExpect(UNITY_LINE_TYPE cmock_line);
typedef void (* CMOCK_taskENTER_CRITICAL_CALLBACK)(int cmock_num_calls);
void taskENTER_CRITICAL_AddCallback(CMOCK_taskENTER_CRITICAL_CALLBACK Callback);
void taskENTER_CRITICAL_Stub(CMOCK_taskENTER_CRITICAL_CALLBACK Callback);
#define taskENTER_CRITICAL_StubWithCallback taskENTER_CRITICAL_Stub
#define taskENTER_CRITICAL_ExpectAndThrow(cmock_to_throw) taskENTER_CRITICAL_CMockExpectAndThrow(__LINE__, cmock_to_throw)
void taskENTER_CRITICAL_CMockExpectAndThrow(UNITY_LINE_TYPE cmock_line, CEXCEPTION_T cmock_to_throw);
#define taskEXIT_CRITICAL_Ignore() taskEXIT_CRITICAL_CMockIgnore()
void taskEXIT_CRITICAL_CMockIgnore(void);
#define taskEXIT_CRITICAL_Expect() taskEXIT_CRITICAL_CMockExpect(__LINE__)
void taskEXIT_CRITICAL_CMockExpect(UNITY_LINE_TYPE cmock_line);
typedef void (* CMOCK_taskEXIT_CRITICAL_CALLBACK)(int cmock_num_calls);
void taskEXIT_CRITICAL_AddCallback(CMOCK_taskEXIT_CRITICAL_CALLBACK Callback);
void taskEXIT_CRITICAL_Stub(CMOCK_taskEXIT_CRITICAL_CALLBACK Callback);
#define taskEXIT_CRITICAL_StubWithCallback taskEXIT_CRITICAL_Stub
#define taskEXIT_CRITICAL_ExpectAndThrow(cmock_to_throw) taskEXIT_CRITICAL_CMockExpectAndThrow(__LINE__, cmock_to_throw)
void taskEXIT_CRITICAL_CMockExpectAndThrow(UNITY_LINE_TYPE cmock_line, CEXCEPTION_T cmock_to_throw);
#define xTaskGetTickCount_IgnoreAndReturn(cmock_retval) xTaskGetTickCount_CMockIgnoreAndReturn(__LINE__, cmock_retval)
void xTaskGetTickCount_CMockIgnoreAndReturn(UNITY_LINE_TYPE cmock_line, TickType_t cmock_to_return);
#define xTaskGetTickCount_ExpectAndReturn(cmock_retval) xTaskGetTickCount_CMockExpectAndReturn(__LINE__, cmock_retval)
void xTaskGetTickCount_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, TickType_t cmock_to_return);
typedef TickType_t (* CMOCK_xTaskGetTickCount_CALLBACK)(int cmock_num_calls);
void xTaskGetTickCount_AddCallback(CMOCK_xTaskGetTickCount_CALLBACK Callback);
void xTaskGetTickCount_Stub(CMOCK_xTaskGetTickCount_CALLBACK Callback);
#define xTaskGetTickCount_StubWithCallback xTaskGetTickCount_Stub
#define xTaskGetTickCount_ExpectAndThrow(cmock_to_throw) xTaskGetTickCount_CMockExpectAndThrow(__LINE__, cmock_to_throw)
void xTaskGetTickCount_CMockExpectAndThrow(UNITY_LINE_TYPE cmock_line, CEXCEPTION_T cmock_to_throw);
#define xTaskGetTickCountFromISR_IgnoreAndReturn(cmock_retval) xTaskGetTickCountFromISR_CMockIgnoreAndReturn(__LINE__, cmock_retval)
void xTaskGetTickCountFromISR_CMockIgnoreAndReturn(UNITY_LINE_TYPE cmock_line, TickType_t cmock_to_return);
#define xTaskGetTickCountFromISR_ExpectAndReturn(cmock_retval) xTaskGetTickCountFromISR_CMockExpectAndReturn(__LINE__, cmock_retval)
void xTaskGetTickCountFromISR_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, TickType_t cmock_to_return);
typedef TickType_t (* CMOCK_xTaskGetTickCountFromISR_CALLBACK)(int cmock_num_calls);
void xTaskGetTickCountFromISR_AddCallback(CMOCK_xTaskGetTickCountFromISR_CALLBACK Callback);
void xTaskGetTickCountFromISR_Stub(CMOCK_xTaskGetTickCountFromISR_CALLBACK Callback);
#define xTaskGetTickCountFromISR_StubWithCallback xTaskGetTickCountFromISR_Stub
#define xTaskGetTickCountFromISR_ExpectAndThrow(cmock_to_throw) xTaskGetTickCountFromISR_CMockExpectAndThrow(__LINE__, cmock_to_throw)
void xTaskGetTickCountFromISR_CMockExpectAndThrow(UNITY_LINE_TYPE cmock_line, CEXCEPTION_T cmock_to_throw);
#define vTaskDelay_Ignore() vTaskDelay_CMockIgnore()
void vTaskDelay_CMockIgnore(void);
#define vTaskDelay_ExpectAnyArgs() vTaskDelay_CMockExpectAnyArgs(__LINE__)
void vTaskDelay_CMockExpectAnyArgs(UNITY_LINE_TYPE cmock_line);
#define vTaskDelay_Expect(xTicksToDelay) vTaskDelay_CMockExpect(__LINE__, xTicksToDelay)
void vTaskDelay_CMockExpect(UNITY_LINE_TYPE cmock_line, const TickType_t xTicksToDelay);
typedef void (* CMOCK_vTaskDelay_CALLBACK)(const TickType_t xTicksToDelay, int cmock_num_calls);
void vTaskDelay_AddCallback(CMOCK_vTaskDelay_CALLBACK Callback);
void vTaskDelay_Stub(CMOCK_vTaskDelay_CALLBACK Callback);
#define vTaskDelay_StubWithCallback vTaskDelay_Stub
#define vTaskDelay_ExpectAndThrow(xTicksToDelay, cmock_to_throw) vTaskDelay_CMockExpectAndThrow(__LINE__, xTicksToDelay, cmock_to_throw)
void vTaskDelay_CMockExpectAndThrow(UNITY_LINE_TYPE cmock_line, const TickType_t xTicksToDelay, CEXCEPTION_T cmock_to_throw);
#define vTaskDelay_IgnoreArg_xTicksToDelay() vTaskDelay_CMockIgnoreArg_xTicksToDelay(__LINE__)
void vTaskDelay_CMockIgnoreArg_xTicksToDelay(UNITY_LINE_TYPE cmock_line);
#define vTaskDelayUntil_Ignore() vTaskDelayUntil_CMockIgnore()
void vTaskDelayUntil_CMockIgnore(void);
#define vTaskDelayUntil_ExpectAnyArgs() vTaskDelayUntil_CMockExpectAnyArgs(__LINE__)
void vTaskDelayUntil_CMockExpectAnyArgs(UNITY_LINE_TYPE cmock_line);
#define vTaskDelayUntil_Expect(pxPreviousWakeTime, xTimeIncrement) vTaskDelayUntil_CMockExpect(__LINE__, pxPreviousWakeTime, xTimeIncrement)
void vTaskDelayUntil_CMockExpect(UNITY_LINE_TYPE cmock_line, TickType_t* const pxPreviousWakeTime, const TickType_t xTimeIncrement);
typedef void (* CMOCK_vTaskDelayUntil_CALLBACK)(TickType_t* const pxPreviousWakeTime, const TickType_t xTimeIncrement, int cmock_num_calls);
void vTaskDelayUntil_AddCallback(CMOCK_vTaskDelayUntil_CALLBACK Callback);
void vTaskDelayUntil_Stub(CMOCK_vTaskDelayUntil_CALLBACK Callback);
#define vTaskDelayUntil_StubWithCallback vTaskDelayUntil_Stub
#define vTaskDelayUntil_ExpectAndThrow(pxPreviousWakeTime, xTimeIncrement, cmock_to_throw) vTaskDelayUntil_CMockExpectAndThrow(__LINE__, pxPreviousWakeTime, xTimeIncrement, cmock_to_throw)
void vTaskDelayUntil_CMockExpectAndThrow(UNITY_LINE_TYPE cmock_line, TickType_t* const pxPreviousWakeTime, const TickType_t xTimeIncrement, CEXCEPTION_T cmock_to_throw);
#define vTaskDelayUntil_ExpectWithArray(pxPreviousWakeTime, pxPreviousWakeTime_Depth, xTimeIncrement) vTaskDelayUntil_CMockExpectWithArray(__LINE__, pxPreviousWakeTime, pxPreviousWakeTime_Depth, xTimeIncrement)
void vTaskDelayUntil_CMockExpectWithArray(UNITY_LINE_TYPE cmock_line, TickType_t* const pxPreviousWakeTime, int pxPreviousWakeTime_Depth, const TickType_t xTimeIncrement);
#define vTaskDelayUntil_ReturnThruPtr_pxPreviousWakeTime(pxPreviousWakeTime) vTaskDelayUntil_CMockReturnMemThruPtr_pxPreviousWakeTime(__LINE__, pxPreviousWakeTime, sizeof(TickType_t))
#define vTaskDelayUntil_ReturnArrayThruPtr_pxPreviousWakeTime(pxPreviousWakeTime, cmock_len) vTaskDelayUntil_CMockReturnMemThruPtr_pxPreviousWakeTime(__LINE__, pxPreviousWakeTime, (int)(cmock_len * (int)sizeof(*pxPreviousWakeTime)))
#define vTaskDelayUntil_ReturnMemThruPtr_pxPreviousWakeTime(pxPreviousWakeTime, cmock_size) vTaskDelayUntil_CMockReturnMemThruPtr_pxPreviousWakeTime(__LINE__, pxPreviousWakeTime, cmock_size)
void vTaskDelayUntil_CMockReturnMemThruPtr_pxPreviousWakeTime(UNITY_LINE_TYPE cmock_line, TickType_t* pxPreviousWakeTime, int cmock_size);
#define vTaskDelayUntil_IgnoreArg_pxPreviousWakeTime() vTaskDelayUntil_CMockIgnoreArg_pxPreviousWakeTime(__LINE__)
void vTaskDelayUntil_CMockIgnoreArg_pxPreviousWakeTime(UNITY_LINE_TYPE cmock_line);
#define vTaskDelayUntil_IgnoreArg_xTimeIncrement() vTaskDelayUntil_CMockIgnoreArg_xTimeIncrement(__LINE__)
void vTaskDelayUntil_CMockIgnoreArg_xTimeIncrement(UNITY_LINE_TYPE cmock_line);
#define xTaskCreate_IgnoreAndReturn(cmock_retval) xTaskCreate_CMockIgnoreAndReturn(__LINE__, cmock_retval)
void xTaskCreate_CMockIgnoreAndReturn(UNITY_LINE_TYPE cmock_line, BaseType_t cmock_to_return);
#define xTaskCreate_ExpectAnyArgsAndReturn(cmock_retval) xTaskCreate_CMockExpectAnyArgsAndReturn(__LINE__, cmock_retval)
void xTaskCreate_CMockExpectAnyArgsAndReturn(UNITY_LINE_TYPE cmock_line, BaseType_t cmock_to_return);
#define xTaskCreate_ExpectAndReturn(pxTaskCode, pcName, usStackDepth, pvParameters, uxPriority, pxCreatedTask, cmock_retval) xTaskCreate_CMockExpectAndReturn(__LINE__, pxTaskCode, pcName, usStackDepth, pvParameters, uxPriority, pxCreatedTask, cmock_retval)
void xTaskCreate_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, TaskFunction_t pxTaskCode, const char* const pcName, const configSTACK_DEPTH_TYPE usStackDepth, void* const pvParameters, UBaseType_t uxPriority, TaskHandle_t* const pxCreatedTask, BaseType_t cmock_to_return);
typedef BaseType_t (* CMOCK_xTaskCreate_CALLBACK)(TaskFunction_t pxTaskCode, const char* const pcName, const configSTACK_DEPTH_TYPE usStackDepth, void* const pvParameters, UBaseType_t uxPriority, TaskHandle_t* const pxCreatedTask, int cmock_num_calls);
void xTaskCreate_AddCallback(CMOCK_xTaskCreate_CALLBACK Callback);
void xTaskCreate_Stub(CMOCK_xTaskCreate_CALLBACK Callback);
#define xTaskCreate_StubWithCallback xTaskCreate_Stub
#define xTaskCreate_ExpectAndThrow(pxTaskCode, pcName, usStackDepth, pvParameters, uxPriority, pxCreatedTask, cmock_to_throw) xTaskCreate_CMockExpectAndThrow(__LINE__, pxTaskCode, pcName, usStackDepth, pvParameters, uxPriority, pxCreatedTask, cmock_to_throw)
void xTaskCreate_CMockExpectAndThrow(UNITY_LINE_TYPE cmock_line, TaskFunction_t pxTaskCode, const char* const pcName, const configSTACK_DEPTH_TYPE usStackDepth, void* const pvParameters, UBaseType_t uxPriority, TaskHandle_t* const pxCreatedTask, CEXCEPTION_T cmock_to_throw);
#define xTaskCreate_ExpectWithArrayAndReturn(pxTaskCode, pcName, usStackDepth, pvParameters, pvParameters_Depth, uxPriority, pxCreatedTask, pxCreatedTask_Depth, cmock_retval) xTaskCreate_CMockExpectWithArrayAndReturn(__LINE__, pxTaskCode, pcName, usStackDepth, pvParameters, pvParameters_Depth, uxPriority, pxCreatedTask, pxCreatedTask_Depth, cmock_retval)
void xTaskCreate_CMockExpectWithArrayAndReturn(UNITY_LINE_TYPE cmock_line, TaskFunction_t pxTaskCode, const char* const pcName, const configSTACK_DEPTH_TYPE usStackDepth, void* const pvParameters, int pvParameters_Depth, UBaseType_t uxPriority, TaskHandle_t* const pxCreatedTask, int pxCreatedTask_Depth, BaseType_t cmock_to_return);
#define xTaskCreate_ReturnThruPtr_pvParameters(pvParameters) xTaskCreate_CMockReturnMemThruPtr_pvParameters(__LINE__, pvParameters, sizeof(void))
#define xTaskCreate_ReturnArrayThruPtr_pvParameters(pvParameters, cmock_len) xTaskCreate_CMockReturnMemThruPtr_pvParameters(__LINE__, pvParameters, (int)(cmock_len * (int)sizeof(*pvParameters)))
#define xTaskCreate_ReturnMemThruPtr_pvParameters(pvParameters, cmock_size) xTaskCreate_CMockReturnMemThruPtr_pvParameters(__LINE__, pvParameters, cmock_size)
void xTaskCreate_CMockReturnMemThruPtr_pvParameters(UNITY_LINE_TYPE cmock_line, void* pvParameters, int cmock_size);
#define xTaskCreate_ReturnThruPtr_pxCreatedTask(pxCreatedTask) xTaskCreate_CMockReturnMemThruPtr_pxCreatedTask(__LINE__, pxCreatedTask, sizeof(TaskHandle_t))
#define xTaskCreate_ReturnArrayThruPtr_pxCreatedTask(pxCreatedTask, cmock_len) xTaskCreate_CMockReturnMemThruPtr_pxCreatedTask(__LINE__, pxCreatedTask, (int)(cmock_len * (int)sizeof(*pxCreatedTask)))
#define xTaskCreate_ReturnMemThruPtr_pxCreatedTask(pxCreatedTask, cmock_size) xTaskCreate_CMockReturnMemThruPtr_pxCreatedTask(__LINE__, pxCreatedTask, cmock_size)
void xTaskCreate_CMockReturnMemThruPtr_pxCreatedTask(UNITY_LINE_TYPE cmock_line, TaskHandle_t* pxCreatedTask, int cmock_size);
#define xTaskCreate_IgnoreArg_pxTaskCode() xTaskCreate_CMockIgnoreArg_pxTaskCode(__LINE__)
void xTaskCreate_CMockIgnoreArg_pxTaskCode(UNITY_LINE_TYPE cmock_line);
#define xTaskCreate_IgnoreArg_pcName() xTaskCreate_CMockIgnoreArg_pcName(__LINE__)
void xTaskCreate_CMockIgnoreArg_pcName(UNITY_LINE_TYPE cmock_line);
#define xTaskCreate_IgnoreArg_usStackDepth() xTaskCreate_CMockIgnoreArg_usStackDepth(__LINE__)
void xTaskCreate_CMockIgnoreArg_usStackDepth(UNITY_LINE_TYPE cmock_line);
#define xTaskCreate_IgnoreArg_pvParameters() xTaskCreate_CMockIgnoreArg_pvParameters(__LINE__)
void xTaskCreate_CMockIgnoreArg_pvParameters(UNITY_LINE_TYPE cmock_line);
#define xTaskCreate_IgnoreArg_uxPriority() xTaskCreate_CMockIgnoreArg_uxPriority(__LINE__)
void xTaskCreate_CMockIgnoreArg_uxPriority(UNITY_LINE_TYPE cmock_line);
#define xTaskCreate_IgnoreArg_pxCreatedTask() xTaskCreate_CMockIgnoreArg_pxCreatedTask(__LINE__)
void xTaskCreate_CMockIgnoreArg_pxCreatedTask(UNITY_LINE_TYPE cmock_line);

#if defined(__GNUC__) && !defined(__ICC) && !defined(__TMS470__)
#if __GNUC__ > 4 || (__GNUC__ == 4 && (__GNUC_MINOR__ > 6 || (__GNUC_MINOR__ == 6 && __GNUC_PATCHLEVEL__ > 0)))
#pragma GCC diagnostic pop
#endif
#endif

#endif
