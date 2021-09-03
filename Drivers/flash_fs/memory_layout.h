
#ifndef NVS_MEMORY_LAYOUT_H
#define NVS_MEMORY_LAYOUT_H

/* Memory map for STM32F407 */
/* mm_page 1 		- 16k : [0x00010000 ... 0x13fff]	*/
/* mm_page 2		- 16k : [0x00014000 ... 0x17fff]	 */
/* page 1 */
#define MEMORY_MANAGER1_OFFSET	0x00010000
#define MEMORY_MANAGER1_LENGTH	0x4000

/* page 2 */
#define MEMORY_MANAGER2_OFFSET	0x00014000
#define MEMORY_MANAGER2_LENGTH	0x4000

#endif /* NVS_MEMORY_LAYOUT_H */
