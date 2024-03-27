/**
  ******************************************************************************
  * @file    network.c
  * @author  AST Embedded Analytics Research Platform
  * @date    Wed Mar 20 21:44:27 2024
  * @brief   AI Tool Automatic Code Generator for Embedded NN computing
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  ******************************************************************************
  */


#include "network.h"
#include "network_data.h"

#include "ai_platform.h"
#include "ai_platform_interface.h"
#include "ai_math_helpers.h"

#include "core_common.h"
#include "core_convert.h"

#include "layers.h"



#undef AI_NET_OBJ_INSTANCE
#define AI_NET_OBJ_INSTANCE g_network
 
#undef AI_NETWORK_MODEL_SIGNATURE
#define AI_NETWORK_MODEL_SIGNATURE     "cfa2c155721d51e6b34add198471acbb"

#ifndef AI_TOOLS_REVISION_ID
#define AI_TOOLS_REVISION_ID     ""
#endif

#undef AI_TOOLS_DATE_TIME
#define AI_TOOLS_DATE_TIME   "Wed Mar 20 21:44:27 2024"

#undef AI_TOOLS_COMPILE_TIME
#define AI_TOOLS_COMPILE_TIME    __DATE__ " " __TIME__

#undef AI_NETWORK_N_BATCHES
#define AI_NETWORK_N_BATCHES         (1)

static ai_ptr g_network_activations_map[1] = AI_C_ARRAY_INIT;
static ai_ptr g_network_weights_map[1] = AI_C_ARRAY_INIT;



/**  Array declarations section  **********************************************/
/* Array#0 */
AI_ARRAY_OBJ_DECLARE(
  dense_20_dense_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 8, AI_STATIC)
/* Array#1 */
AI_ARRAY_OBJ_DECLARE(
  dense_21_dense_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 8, AI_STATIC)
/* Array#2 */
AI_ARRAY_OBJ_DECLARE(
  dense_21_dense_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 1, AI_STATIC)
/* Array#3 */
AI_ARRAY_OBJ_DECLARE(
  lstm_20_scratch0_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 350, AI_STATIC)
/* Array#4 */
AI_ARRAY_OBJ_DECLARE(
  lstm_21_scratch0_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 350, AI_STATIC)
/* Array#5 */
AI_ARRAY_OBJ_DECLARE(
  lstm_22_scratch0_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 350, AI_STATIC)
/* Array#6 */
AI_ARRAY_OBJ_DECLARE(
  input_0_output_array, AI_ARRAY_FORMAT_FLOAT|AI_FMT_FLAG_IS_IO,
  NULL, NULL, 2, AI_STATIC)
/* Array#7 */
AI_ARRAY_OBJ_DECLARE(
  lstm_20_output0_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 50, AI_STATIC)
/* Array#8 */
AI_ARRAY_OBJ_DECLARE(
  lstm_21_output0_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 50, AI_STATIC)
/* Array#9 */
AI_ARRAY_OBJ_DECLARE(
  lstm_22_output0_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 50, AI_STATIC)
/* Array#10 */
AI_ARRAY_OBJ_DECLARE(
  dense_20_dense_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 8, AI_STATIC)
/* Array#11 */
AI_ARRAY_OBJ_DECLARE(
  dense_20_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 8, AI_STATIC)
/* Array#12 */
AI_ARRAY_OBJ_DECLARE(
  dense_21_dense_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 1, AI_STATIC)
/* Array#13 */
AI_ARRAY_OBJ_DECLARE(
  dense_21_output_array, AI_ARRAY_FORMAT_FLOAT|AI_FMT_FLAG_IS_IO,
  NULL, NULL, 1, AI_STATIC)
/* Array#14 */
AI_ARRAY_OBJ_DECLARE(
  lstm_20_kernel_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 400, AI_STATIC)
/* Array#15 */
AI_ARRAY_OBJ_DECLARE(
  lstm_20_recurrent_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 10000, AI_STATIC)
/* Array#16 */
AI_ARRAY_OBJ_DECLARE(
  lstm_20_peephole_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 150, AI_STATIC)
/* Array#17 */
AI_ARRAY_OBJ_DECLARE(
  lstm_20_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 200, AI_STATIC)
/* Array#18 */
AI_ARRAY_OBJ_DECLARE(
  lstm_21_kernel_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 10000, AI_STATIC)
/* Array#19 */
AI_ARRAY_OBJ_DECLARE(
  lstm_21_recurrent_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 10000, AI_STATIC)
/* Array#20 */
AI_ARRAY_OBJ_DECLARE(
  lstm_21_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 200, AI_STATIC)
/* Array#21 */
AI_ARRAY_OBJ_DECLARE(
  lstm_22_kernel_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 10000, AI_STATIC)
/* Array#22 */
AI_ARRAY_OBJ_DECLARE(
  lstm_22_recurrent_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 10000, AI_STATIC)
/* Array#23 */
AI_ARRAY_OBJ_DECLARE(
  lstm_22_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 200, AI_STATIC)
/* Array#24 */
AI_ARRAY_OBJ_DECLARE(
  dense_20_dense_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 400, AI_STATIC)
/**  Tensor declarations section  *********************************************/
/* Tensor #0 */
AI_TENSOR_OBJ_DECLARE(
  dense_20_dense_bias, AI_STATIC,
  0, 0x0,
  AI_SHAPE_INIT(4, 1, 8, 1, 1), AI_STRIDE_INIT(4, 4, 4, 32, 32),
  1, &dense_20_dense_bias_array, NULL)

/* Tensor #1 */
AI_TENSOR_OBJ_DECLARE(
  dense_21_dense_weights, AI_STATIC,
  1, 0x0,
  AI_SHAPE_INIT(4, 8, 1, 1, 1), AI_STRIDE_INIT(4, 4, 32, 32, 32),
  1, &dense_21_dense_weights_array, NULL)

/* Tensor #2 */
AI_TENSOR_OBJ_DECLARE(
  dense_21_dense_bias, AI_STATIC,
  2, 0x0,
  AI_SHAPE_INIT(4, 1, 1, 1, 1), AI_STRIDE_INIT(4, 4, 4, 4, 4),
  1, &dense_21_dense_bias_array, NULL)

/* Tensor #3 */
AI_TENSOR_OBJ_DECLARE(
  lstm_20_scratch0, AI_STATIC,
  3, 0x0,
  AI_SHAPE_INIT(4, 1, 350, 1, 1), AI_STRIDE_INIT(4, 4, 4, 1400, 1400),
  1, &lstm_20_scratch0_array, NULL)

/* Tensor #4 */
AI_TENSOR_OBJ_DECLARE(
  lstm_21_scratch0, AI_STATIC,
  4, 0x0,
  AI_SHAPE_INIT(4, 1, 350, 1, 1), AI_STRIDE_INIT(4, 4, 4, 1400, 1400),
  1, &lstm_21_scratch0_array, NULL)

/* Tensor #5 */
AI_TENSOR_OBJ_DECLARE(
  lstm_22_scratch0, AI_STATIC,
  5, 0x0,
  AI_SHAPE_INIT(4, 1, 350, 1, 1), AI_STRIDE_INIT(4, 4, 4, 1400, 1400),
  1, &lstm_22_scratch0_array, NULL)

/* Tensor #6 */
AI_TENSOR_OBJ_DECLARE(
  input_0_output, AI_STATIC,
  6, 0x0,
  AI_SHAPE_INIT(4, 1, 2, 1, 1), AI_STRIDE_INIT(4, 4, 4, 8, 8),
  1, &input_0_output_array, NULL)

/* Tensor #7 */
AI_TENSOR_OBJ_DECLARE(
  lstm_20_output0, AI_STATIC,
  7, 0x0,
  AI_SHAPE_INIT(4, 1, 50, 1, 1), AI_STRIDE_INIT(4, 4, 4, 200, 200),
  1, &lstm_20_output0_array, NULL)

/* Tensor #8 */
AI_TENSOR_OBJ_DECLARE(
  lstm_21_output0, AI_STATIC,
  8, 0x0,
  AI_SHAPE_INIT(4, 1, 50, 1, 1), AI_STRIDE_INIT(4, 4, 4, 200, 200),
  1, &lstm_21_output0_array, NULL)

/* Tensor #9 */
AI_TENSOR_OBJ_DECLARE(
  lstm_22_output0, AI_STATIC,
  9, 0x0,
  AI_SHAPE_INIT(4, 1, 50, 1, 1), AI_STRIDE_INIT(4, 4, 4, 200, 200),
  1, &lstm_22_output0_array, NULL)

/* Tensor #10 */
AI_TENSOR_OBJ_DECLARE(
  dense_20_dense_output, AI_STATIC,
  10, 0x0,
  AI_SHAPE_INIT(4, 1, 8, 1, 1), AI_STRIDE_INIT(4, 4, 4, 32, 32),
  1, &dense_20_dense_output_array, NULL)

/* Tensor #11 */
AI_TENSOR_OBJ_DECLARE(
  dense_20_output, AI_STATIC,
  11, 0x0,
  AI_SHAPE_INIT(4, 1, 8, 1, 1), AI_STRIDE_INIT(4, 4, 4, 32, 32),
  1, &dense_20_output_array, NULL)

/* Tensor #12 */
AI_TENSOR_OBJ_DECLARE(
  dense_21_dense_output, AI_STATIC,
  12, 0x0,
  AI_SHAPE_INIT(4, 1, 1, 1, 1), AI_STRIDE_INIT(4, 4, 4, 4, 4),
  1, &dense_21_dense_output_array, NULL)

/* Tensor #13 */
AI_TENSOR_OBJ_DECLARE(
  dense_21_output, AI_STATIC,
  13, 0x0,
  AI_SHAPE_INIT(4, 1, 1, 1, 1), AI_STRIDE_INIT(4, 4, 4, 4, 4),
  1, &dense_21_output_array, NULL)

/* Tensor #14 */
AI_TENSOR_OBJ_DECLARE(
  lstm_20_kernel, AI_STATIC,
  14, 0x0,
  AI_SHAPE_INIT(4, 2, 200, 1, 1), AI_STRIDE_INIT(4, 4, 8, 1600, 1600),
  1, &lstm_20_kernel_array, NULL)

/* Tensor #15 */
AI_TENSOR_OBJ_DECLARE(
  lstm_20_recurrent, AI_STATIC,
  15, 0x0,
  AI_SHAPE_INIT(4, 50, 200, 1, 1), AI_STRIDE_INIT(4, 4, 200, 40000, 40000),
  1, &lstm_20_recurrent_array, NULL)

/* Tensor #16 */
AI_TENSOR_OBJ_DECLARE(
  lstm_20_peephole, AI_STATIC,
  16, 0x0,
  AI_SHAPE_INIT(4, 1, 1, 1, 150), AI_STRIDE_INIT(4, 4, 4, 600, 600),
  1, &lstm_20_peephole_array, NULL)

/* Tensor #17 */
AI_TENSOR_OBJ_DECLARE(
  lstm_20_bias, AI_STATIC,
  17, 0x0,
  AI_SHAPE_INIT(4, 1, 200, 1, 1), AI_STRIDE_INIT(4, 4, 4, 800, 800),
  1, &lstm_20_bias_array, NULL)

/* Tensor #18 */
AI_TENSOR_OBJ_DECLARE(
  lstm_21_kernel, AI_STATIC,
  18, 0x0,
  AI_SHAPE_INIT(4, 50, 200, 1, 1), AI_STRIDE_INIT(4, 4, 200, 40000, 40000),
  1, &lstm_21_kernel_array, NULL)

/* Tensor #19 */
AI_TENSOR_OBJ_DECLARE(
  lstm_21_recurrent, AI_STATIC,
  19, 0x0,
  AI_SHAPE_INIT(4, 50, 200, 1, 1), AI_STRIDE_INIT(4, 4, 200, 40000, 40000),
  1, &lstm_21_recurrent_array, NULL)

/* Tensor #20 */
AI_TENSOR_OBJ_DECLARE(
  lstm_21_bias, AI_STATIC,
  20, 0x0,
  AI_SHAPE_INIT(4, 1, 200, 1, 1), AI_STRIDE_INIT(4, 4, 4, 800, 800),
  1, &lstm_21_bias_array, NULL)

/* Tensor #21 */
AI_TENSOR_OBJ_DECLARE(
  lstm_22_kernel, AI_STATIC,
  21, 0x0,
  AI_SHAPE_INIT(4, 50, 200, 1, 1), AI_STRIDE_INIT(4, 4, 200, 40000, 40000),
  1, &lstm_22_kernel_array, NULL)

/* Tensor #22 */
AI_TENSOR_OBJ_DECLARE(
  lstm_22_recurrent, AI_STATIC,
  22, 0x0,
  AI_SHAPE_INIT(4, 50, 200, 1, 1), AI_STRIDE_INIT(4, 4, 200, 40000, 40000),
  1, &lstm_22_recurrent_array, NULL)

/* Tensor #23 */
AI_TENSOR_OBJ_DECLARE(
  lstm_22_bias, AI_STATIC,
  23, 0x0,
  AI_SHAPE_INIT(4, 1, 200, 1, 1), AI_STRIDE_INIT(4, 4, 4, 800, 800),
  1, &lstm_22_bias_array, NULL)

/* Tensor #24 */
AI_TENSOR_OBJ_DECLARE(
  dense_20_dense_weights, AI_STATIC,
  24, 0x0,
  AI_SHAPE_INIT(4, 50, 8, 1, 1), AI_STRIDE_INIT(4, 4, 200, 1600, 1600),
  1, &dense_20_dense_weights_array, NULL)



/**  Layer declarations section  **********************************************/


AI_TENSOR_CHAIN_OBJ_DECLARE(
  dense_21_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &dense_21_dense_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &dense_21_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  dense_21_layer, 4,
  NL_TYPE, 0x0, NULL,
  nl, forward_relu,
  &dense_21_chain,
  NULL, &dense_21_layer, AI_STATIC, 
  .nl_params = NULL, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  dense_21_dense_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &dense_20_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &dense_21_dense_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &dense_21_dense_weights, &dense_21_dense_bias),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  dense_21_dense_layer, 4,
  DENSE_TYPE, 0x0, NULL,
  dense, forward_dense,
  &dense_21_dense_chain,
  NULL, &dense_21_layer, AI_STATIC, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  dense_20_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &dense_20_dense_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &dense_20_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  dense_20_layer, 3,
  NL_TYPE, 0x0, NULL,
  nl, forward_relu,
  &dense_20_chain,
  NULL, &dense_21_dense_layer, AI_STATIC, 
  .nl_params = NULL, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  dense_20_dense_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &lstm_22_output0),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &dense_20_dense_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &dense_20_dense_weights, &dense_20_dense_bias),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  dense_20_dense_layer, 3,
  DENSE_TYPE, 0x0, NULL,
  dense, forward_dense,
  &dense_20_dense_chain,
  NULL, &dense_20_layer, AI_STATIC, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  lstm_22_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &lstm_21_output0),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &lstm_22_output0),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 9, &lstm_22_kernel, &lstm_22_recurrent, &lstm_20_peephole, &lstm_22_bias, NULL, NULL, NULL, NULL, NULL),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &lstm_22_scratch0)
)

AI_LAYER_OBJ_DECLARE(
  lstm_22_layer, 2,
  LSTM_TYPE, 0x0, NULL,
  lstm, forward_lstm,
  &lstm_22_chain,
  NULL, &dense_20_dense_layer, AI_STATIC, 
  .n_units = 50, 
  .activation_nl = nl_func_tanh_array_f32, 
  .go_backwards = false, 
  .reverse_seq = false, 
  .return_state = false, 
  .out_nl = nl_func_tanh_array_f32, 
  .recurrent_nl = nl_func_sigmoid_array_f32, 
  .cell_clip = 3e+38, 
  .state = AI_HANDLE_PTR(NULL), 
  .init = AI_LAYER_FUNC(NULL), 
  .destroy = AI_LAYER_FUNC(NULL), 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  lstm_21_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &lstm_20_output0),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &lstm_21_output0),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 9, &lstm_21_kernel, &lstm_21_recurrent, &lstm_20_peephole, &lstm_21_bias, NULL, NULL, NULL, NULL, NULL),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &lstm_21_scratch0)
)

AI_LAYER_OBJ_DECLARE(
  lstm_21_layer, 1,
  LSTM_TYPE, 0x0, NULL,
  lstm, forward_lstm,
  &lstm_21_chain,
  NULL, &lstm_22_layer, AI_STATIC, 
  .n_units = 50, 
  .activation_nl = nl_func_tanh_array_f32, 
  .go_backwards = false, 
  .reverse_seq = false, 
  .return_state = false, 
  .out_nl = nl_func_tanh_array_f32, 
  .recurrent_nl = nl_func_sigmoid_array_f32, 
  .cell_clip = 3e+38, 
  .state = AI_HANDLE_PTR(NULL), 
  .init = AI_LAYER_FUNC(NULL), 
  .destroy = AI_LAYER_FUNC(NULL), 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  lstm_20_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &input_0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &lstm_20_output0),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 9, &lstm_20_kernel, &lstm_20_recurrent, &lstm_20_peephole, &lstm_20_bias, NULL, NULL, NULL, NULL, NULL),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &lstm_20_scratch0)
)

AI_LAYER_OBJ_DECLARE(
  lstm_20_layer, 0,
  LSTM_TYPE, 0x0, NULL,
  lstm, forward_lstm,
  &lstm_20_chain,
  NULL, &lstm_21_layer, AI_STATIC, 
  .n_units = 50, 
  .activation_nl = nl_func_tanh_array_f32, 
  .go_backwards = false, 
  .reverse_seq = false, 
  .return_state = false, 
  .out_nl = nl_func_tanh_array_f32, 
  .recurrent_nl = nl_func_sigmoid_array_f32, 
  .cell_clip = 3e+38, 
  .state = AI_HANDLE_PTR(NULL), 
  .init = AI_LAYER_FUNC(NULL), 
  .destroy = AI_LAYER_FUNC(NULL), 
)


#if (AI_TOOLS_API_VERSION < AI_TOOLS_API_VERSION_1_5)

AI_NETWORK_OBJ_DECLARE(
  AI_NET_OBJ_INSTANCE, AI_STATIC,
  AI_BUFFER_INIT(AI_FLAG_NONE,  AI_BUFFER_FORMAT_U8,
    AI_BUFFER_SHAPE_INIT(AI_SHAPE_BCWH, 4, 1, 206268, 1, 1),
    206268, NULL, NULL),
  AI_BUFFER_INIT(AI_FLAG_NONE,  AI_BUFFER_FORMAT_U8,
    AI_BUFFER_SHAPE_INIT(AI_SHAPE_BCWH, 4, 1, 1800, 1, 1),
    1800, NULL, NULL),
  AI_TENSOR_LIST_IO_OBJ_INIT(AI_FLAG_NONE, AI_NETWORK_IN_NUM, &input_0_output),
  AI_TENSOR_LIST_IO_OBJ_INIT(AI_FLAG_NONE, AI_NETWORK_OUT_NUM, &dense_21_output),
  &lstm_20_layer, 0, NULL)

#else

AI_NETWORK_OBJ_DECLARE(
  AI_NET_OBJ_INSTANCE, AI_STATIC,
  AI_BUFFER_ARRAY_OBJ_INIT_STATIC(
  	AI_FLAG_NONE, 1,
    AI_BUFFER_INIT(AI_FLAG_NONE,  AI_BUFFER_FORMAT_U8,
      AI_BUFFER_SHAPE_INIT(AI_SHAPE_BCWH, 4, 1, 206268, 1, 1),
      206268, NULL, NULL)
  ),
  AI_BUFFER_ARRAY_OBJ_INIT_STATIC(
  	AI_FLAG_NONE, 1,
    AI_BUFFER_INIT(AI_FLAG_NONE,  AI_BUFFER_FORMAT_U8,
      AI_BUFFER_SHAPE_INIT(AI_SHAPE_BCWH, 4, 1, 1800, 1, 1),
      1800, NULL, NULL)
  ),
  AI_TENSOR_LIST_IO_OBJ_INIT(AI_FLAG_NONE, AI_NETWORK_IN_NUM, &input_0_output),
  AI_TENSOR_LIST_IO_OBJ_INIT(AI_FLAG_NONE, AI_NETWORK_OUT_NUM, &dense_21_output),
  &lstm_20_layer, 0, NULL)

#endif	/*(AI_TOOLS_API_VERSION < AI_TOOLS_API_VERSION_1_5)*/


/******************************************************************************/
AI_DECLARE_STATIC
ai_bool network_configure_activations(
  ai_network* net_ctx, const ai_network_params* params)
{
  AI_ASSERT(net_ctx)

  if (ai_platform_get_activations_map(g_network_activations_map, 1, params)) {
    /* Updating activations (byte) offsets */
    
    input_0_output_array.data = AI_PTR(g_network_activations_map[0] + 192);
    input_0_output_array.data_start = AI_PTR(g_network_activations_map[0] + 192);
    
    lstm_20_scratch0_array.data = AI_PTR(g_network_activations_map[0] + 200);
    lstm_20_scratch0_array.data_start = AI_PTR(g_network_activations_map[0] + 200);
    
    lstm_20_output0_array.data = AI_PTR(g_network_activations_map[0] + 1600);
    lstm_20_output0_array.data_start = AI_PTR(g_network_activations_map[0] + 1600);
    
    lstm_21_scratch0_array.data = AI_PTR(g_network_activations_map[0] + 200);
    lstm_21_scratch0_array.data_start = AI_PTR(g_network_activations_map[0] + 200);
    
    lstm_21_output0_array.data = AI_PTR(g_network_activations_map[0] + 0);
    lstm_21_output0_array.data_start = AI_PTR(g_network_activations_map[0] + 0);
    
    lstm_22_scratch0_array.data = AI_PTR(g_network_activations_map[0] + 200);
    lstm_22_scratch0_array.data_start = AI_PTR(g_network_activations_map[0] + 200);
    
    lstm_22_output0_array.data = AI_PTR(g_network_activations_map[0] + 1600);
    lstm_22_output0_array.data_start = AI_PTR(g_network_activations_map[0] + 1600);
    
    dense_20_dense_output_array.data = AI_PTR(g_network_activations_map[0] + 0);
    dense_20_dense_output_array.data_start = AI_PTR(g_network_activations_map[0] + 0);
    
    dense_20_output_array.data = AI_PTR(g_network_activations_map[0] + 32);
    dense_20_output_array.data_start = AI_PTR(g_network_activations_map[0] + 32);
    
    dense_21_dense_output_array.data = AI_PTR(g_network_activations_map[0] + 0);
    dense_21_dense_output_array.data_start = AI_PTR(g_network_activations_map[0] + 0);
    
    dense_21_output_array.data = AI_PTR(g_network_activations_map[0] + 4);
    dense_21_output_array.data_start = AI_PTR(g_network_activations_map[0] + 4);
    
    return true;
  }
  AI_ERROR_TRAP(net_ctx, INIT_FAILED, NETWORK_ACTIVATIONS);
  return false;
}



/******************************************************************************/
AI_DECLARE_STATIC
ai_bool network_configure_weights(
  ai_network* net_ctx, const ai_network_params* params)
{
  AI_ASSERT(net_ctx)

  if (ai_platform_get_weights_map(g_network_weights_map, 1, params)) {
    /* Updating weights (byte) offsets */
    
    dense_20_dense_bias_array.format |= AI_FMT_FLAG_CONST;
    dense_20_dense_bias_array.data = AI_PTR(g_network_weights_map[0] + 0);
    dense_20_dense_bias_array.data_start = AI_PTR(g_network_weights_map[0] + 0);
    
    dense_21_dense_weights_array.format |= AI_FMT_FLAG_CONST;
    dense_21_dense_weights_array.data = AI_PTR(g_network_weights_map[0] + 32);
    dense_21_dense_weights_array.data_start = AI_PTR(g_network_weights_map[0] + 32);
    
    dense_21_dense_bias_array.format |= AI_FMT_FLAG_CONST;
    dense_21_dense_bias_array.data = AI_PTR(g_network_weights_map[0] + 64);
    dense_21_dense_bias_array.data_start = AI_PTR(g_network_weights_map[0] + 64);
    
    lstm_20_kernel_array.format |= AI_FMT_FLAG_CONST;
    lstm_20_kernel_array.data = AI_PTR(g_network_weights_map[0] + 68);
    lstm_20_kernel_array.data_start = AI_PTR(g_network_weights_map[0] + 68);
    
    lstm_20_recurrent_array.format |= AI_FMT_FLAG_CONST;
    lstm_20_recurrent_array.data = AI_PTR(g_network_weights_map[0] + 1668);
    lstm_20_recurrent_array.data_start = AI_PTR(g_network_weights_map[0] + 1668);
    
    lstm_20_peephole_array.format |= AI_FMT_FLAG_CONST;
    lstm_20_peephole_array.data = AI_PTR(g_network_weights_map[0] + 41668);
    lstm_20_peephole_array.data_start = AI_PTR(g_network_weights_map[0] + 41668);
    
    lstm_20_bias_array.format |= AI_FMT_FLAG_CONST;
    lstm_20_bias_array.data = AI_PTR(g_network_weights_map[0] + 42268);
    lstm_20_bias_array.data_start = AI_PTR(g_network_weights_map[0] + 42268);
    
    lstm_21_kernel_array.format |= AI_FMT_FLAG_CONST;
    lstm_21_kernel_array.data = AI_PTR(g_network_weights_map[0] + 43068);
    lstm_21_kernel_array.data_start = AI_PTR(g_network_weights_map[0] + 43068);
    
    lstm_21_recurrent_array.format |= AI_FMT_FLAG_CONST;
    lstm_21_recurrent_array.data = AI_PTR(g_network_weights_map[0] + 83068);
    lstm_21_recurrent_array.data_start = AI_PTR(g_network_weights_map[0] + 83068);
    
    lstm_21_bias_array.format |= AI_FMT_FLAG_CONST;
    lstm_21_bias_array.data = AI_PTR(g_network_weights_map[0] + 123068);
    lstm_21_bias_array.data_start = AI_PTR(g_network_weights_map[0] + 123068);
    
    lstm_22_kernel_array.format |= AI_FMT_FLAG_CONST;
    lstm_22_kernel_array.data = AI_PTR(g_network_weights_map[0] + 123868);
    lstm_22_kernel_array.data_start = AI_PTR(g_network_weights_map[0] + 123868);
    
    lstm_22_recurrent_array.format |= AI_FMT_FLAG_CONST;
    lstm_22_recurrent_array.data = AI_PTR(g_network_weights_map[0] + 163868);
    lstm_22_recurrent_array.data_start = AI_PTR(g_network_weights_map[0] + 163868);
    
    lstm_22_bias_array.format |= AI_FMT_FLAG_CONST;
    lstm_22_bias_array.data = AI_PTR(g_network_weights_map[0] + 203868);
    lstm_22_bias_array.data_start = AI_PTR(g_network_weights_map[0] + 203868);
    
    dense_20_dense_weights_array.format |= AI_FMT_FLAG_CONST;
    dense_20_dense_weights_array.data = AI_PTR(g_network_weights_map[0] + 204668);
    dense_20_dense_weights_array.data_start = AI_PTR(g_network_weights_map[0] + 204668);
    
    return true;
  }
  AI_ERROR_TRAP(net_ctx, INIT_FAILED, NETWORK_WEIGHTS);
  return false;
}


/**  PUBLIC APIs SECTION  *****************************************************/


AI_DEPRECATED
AI_API_ENTRY
ai_bool ai_network_get_info(
  ai_handle network, ai_network_report* report)
{
  ai_network* net_ctx = AI_NETWORK_ACQUIRE_CTX(network);

  if (report && net_ctx)
  {
    ai_network_report r = {
      .model_name        = AI_NETWORK_MODEL_NAME,
      .model_signature   = AI_NETWORK_MODEL_SIGNATURE,
      .model_datetime    = AI_TOOLS_DATE_TIME,
      
      .compile_datetime  = AI_TOOLS_COMPILE_TIME,
      
      .runtime_revision  = ai_platform_runtime_get_revision(),
      .runtime_version   = ai_platform_runtime_get_version(),

      .tool_revision     = AI_TOOLS_REVISION_ID,
      .tool_version      = {AI_TOOLS_VERSION_MAJOR, AI_TOOLS_VERSION_MINOR,
                            AI_TOOLS_VERSION_MICRO, 0x0},
      .tool_api_version  = AI_STRUCT_INIT,

      .api_version            = ai_platform_api_get_version(),
      .interface_api_version  = ai_platform_interface_api_get_version(),
      
      .n_macc            = 51576,
      .n_inputs          = 0,
      .inputs            = NULL,
      .n_outputs         = 0,
      .outputs           = NULL,
      .params            = AI_STRUCT_INIT,
      .activations       = AI_STRUCT_INIT,
      .n_nodes           = 0,
      .signature         = 0x0,
    };

    if (!ai_platform_api_get_network_report(network, &r)) return false;

    *report = r;
    return true;
  }
  return false;
}


AI_API_ENTRY
ai_bool ai_network_get_report(
  ai_handle network, ai_network_report* report)
{
  ai_network* net_ctx = AI_NETWORK_ACQUIRE_CTX(network);

  if (report && net_ctx)
  {
    ai_network_report r = {
      .model_name        = AI_NETWORK_MODEL_NAME,
      .model_signature   = AI_NETWORK_MODEL_SIGNATURE,
      .model_datetime    = AI_TOOLS_DATE_TIME,
      
      .compile_datetime  = AI_TOOLS_COMPILE_TIME,
      
      .runtime_revision  = ai_platform_runtime_get_revision(),
      .runtime_version   = ai_platform_runtime_get_version(),

      .tool_revision     = AI_TOOLS_REVISION_ID,
      .tool_version      = {AI_TOOLS_VERSION_MAJOR, AI_TOOLS_VERSION_MINOR,
                            AI_TOOLS_VERSION_MICRO, 0x0},
      .tool_api_version  = AI_STRUCT_INIT,

      .api_version            = ai_platform_api_get_version(),
      .interface_api_version  = ai_platform_interface_api_get_version(),
      
      .n_macc            = 51576,
      .n_inputs          = 0,
      .inputs            = NULL,
      .n_outputs         = 0,
      .outputs           = NULL,
      .map_signature     = AI_MAGIC_SIGNATURE,
      .map_weights       = AI_STRUCT_INIT,
      .map_activations   = AI_STRUCT_INIT,
      .n_nodes           = 0,
      .signature         = 0x0,
    };

    if (!ai_platform_api_get_network_report(network, &r)) return false;

    *report = r;
    return true;
  }
  return false;
}

AI_API_ENTRY
ai_error ai_network_get_error(ai_handle network)
{
  return ai_platform_network_get_error(network);
}

AI_API_ENTRY
ai_error ai_network_create(
  ai_handle* network, const ai_buffer* network_config)
{
  return ai_platform_network_create(
    network, network_config, 
    &AI_NET_OBJ_INSTANCE,
    AI_TOOLS_API_VERSION_MAJOR, AI_TOOLS_API_VERSION_MINOR, AI_TOOLS_API_VERSION_MICRO);
}

AI_API_ENTRY
ai_error ai_network_create_and_init(
  ai_handle* network, const ai_handle activations[], const ai_handle weights[])
{
    ai_error err;
    ai_network_params params;

    err = ai_network_create(network, AI_NETWORK_DATA_CONFIG);
    if (err.type != AI_ERROR_NONE)
        return err;
    if (ai_network_data_params_get(&params) != true) {
        err = ai_network_get_error(*network);
        return err;
    }
#if defined(AI_NETWORK_DATA_ACTIVATIONS_COUNT)
    if (activations) {
        /* set the addresses of the activations buffers */
        for (int idx=0;idx<params.map_activations.size;idx++)
            AI_BUFFER_ARRAY_ITEM_SET_ADDRESS(&params.map_activations, idx, activations[idx]);
    }
#endif
#if defined(AI_NETWORK_DATA_WEIGHTS_COUNT)
    if (weights) {
        /* set the addresses of the weight buffers */
        for (int idx=0;idx<params.map_weights.size;idx++)
            AI_BUFFER_ARRAY_ITEM_SET_ADDRESS(&params.map_weights, idx, weights[idx]);
    }
#endif
    if (ai_network_init(*network, &params) != true) {
        err = ai_network_get_error(*network);
    }
    return err;
}

AI_API_ENTRY
ai_buffer* ai_network_inputs_get(ai_handle network, ai_u16 *n_buffer)
{
  if (network == AI_HANDLE_NULL) {
    network = (ai_handle)&AI_NET_OBJ_INSTANCE;
    ((ai_network *)network)->magic = AI_MAGIC_CONTEXT_TOKEN;
  }
  return ai_platform_inputs_get(network, n_buffer);
}

AI_API_ENTRY
ai_buffer* ai_network_outputs_get(ai_handle network, ai_u16 *n_buffer)
{
  if (network == AI_HANDLE_NULL) {
    network = (ai_handle)&AI_NET_OBJ_INSTANCE;
    ((ai_network *)network)->magic = AI_MAGIC_CONTEXT_TOKEN;
  }
  return ai_platform_outputs_get(network, n_buffer);
}

AI_API_ENTRY
ai_handle ai_network_destroy(ai_handle network)
{
  return ai_platform_network_destroy(network);
}

AI_API_ENTRY
ai_bool ai_network_init(
  ai_handle network, const ai_network_params* params)
{
  ai_network* net_ctx = ai_platform_network_init(network, params);
  if (!net_ctx) return false;

  ai_bool ok = true;
  ok &= network_configure_weights(net_ctx, params);
  ok &= network_configure_activations(net_ctx, params);

  ok &= ai_platform_network_post_init(network);

  return ok;
}


AI_API_ENTRY
ai_i32 ai_network_run(
  ai_handle network, const ai_buffer* input, ai_buffer* output)
{
  return ai_platform_network_process(network, input, output);
}

AI_API_ENTRY
ai_i32 ai_network_forward(ai_handle network, const ai_buffer* input)
{
  return ai_platform_network_process(network, input, NULL);
}



#undef AI_NETWORK_MODEL_SIGNATURE
#undef AI_NET_OBJ_INSTANCE
#undef AI_TOOLS_DATE_TIME
#undef AI_TOOLS_COMPILE_TIME

