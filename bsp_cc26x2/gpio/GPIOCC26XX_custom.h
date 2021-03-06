/*
 */
/** ============================================================================
 *  @file       GPIOCC26XX.h
 *
 *  @brief      GPIO driver implementation for CC26xx/CC13xx devices
 *
 *  The GPIO header file should be included in an application as follows:
 *  @code
 *  #include <ti/drivers/GPIO.h>
 *  #include <ti/drivers/gpio/GPIOCC26XX.h>
 *  @endcode
 *
 *  Refer to @ref GPIO.h for a complete description of the GPIO
 *  driver APIs provided and examples of their use.
 *
 *  ### CC26XX GPIO Driver Configuration #
 *
 *  In order to use the GPIO APIs, the application is required
 *  to provide 3 structures in the ti_drivers_config.c file:
 *
 *  1.  An array of @ref GPIO_PinConfig elements that defines the
 *  initial configuration of each pin used by the application. A
 *  pin is referenced in the application by its corresponding index in this
 *  array. The pin type (that is, INPUT/OUTPUT), its initial state (that is
 *  OUTPUT_HIGH or LOW), interrupt behavior (RISING/FALLING edge, etc.)
 *  (see @ref GPIO_PinConfigSettings), and
 *  device specific pin identification (see @ref GPIOCC26XX_PinConfigIds)
 *  are configured in each element of this array.
 *  Below is an CC26XX device specific example of the GPIO_PinConfig array:
 *  @code
 *  //
 *  // Array of Pin configurations
 *  // NOTE: The order of the pin configurations must coincide with what was
 *  //       defined in CC2650_LAUNCH.h
 *  // NOTE: Pins not used for interrupts should be placed at the end of the
 *  //       array.  Callback entries can be omitted from callbacks array to
 *  //       reduce memory usage.
 *  //
 *  GPIO_PinConfig gpioPinConfigs[] = {
 *      // Input pins
 *      GPIOCC26XX_DIO_13 | GPIO_CFG_IN_PU | GPIO_CFG_IN_INT_RISING,  // Button 0
 *      GPIOCC26XX_DIO_14 | GPIO_CFG_IN_PU | GPIO_CFG_IN_INT_RISING,  // Button 1
 *
 *      // Output pins
 *      GPIOCC26XX_DIO_07 | GPIO_CFG_OUT_STD | GPIO_CFG_OUT_STR_HIGH | GPIO_CFG_OUT_LOW,     //  Green LED
 *      GPIOCC26XX_DIO_06 | GPIO_CFG_OUT_STD | GPIO_CFG_OUT_STR_HIGH | GPIO_CFG_OUT_LOW,     //  Red LED
 *  };
 *  @endcode
 *
 *  2.  An array of @ref GPIO_CallbackFxn elements that is used to store
 *  callback function pointers for GPIO pins configured with interrupts.
 *  The indexes for these array elements correspond to the pins defined
 *  in the @ref GPIO_PinConfig array. These function pointers can be defined
 *  statically by referencing the callback function name in the array
 *  element, or dynamically, by setting the array element to NULL and using
 *  GPIO_setCallback() at runtime to plug the callback entry.
 *  Pins not used for interrupts can be omitted from the callback array to
 *  reduce memory usage (if they are placed at the end of the @ref
 *  GPIO_PinConfig array). The callback function syntax should match the
 *  following:
 *  @code
 *  void (*GPIO_CallbackFxn)(unsigned int index);
 *  @endcode
 *  The index parameter is the same index that was passed to
 *  GPIO_setCallback(). This allows the same callback function to be used
 *  for multiple GPIO interrupts, by using the index to identify the GPIO
 *  that caused the interrupt.
 *  Below is an CC26XX device specific example of the @ref GPIO_CallbackFxn
 *  array:
 *  @code
 *  //
 *  // Array of callback function pointers
 *  // NOTE: The order of the pin configurations must coincide with what was
 *  //       defined in CC2650_LAUNCH.h
 *  // NOTE: Pins not used for interrupts can be omitted from callbacks array to
 *  //       reduce memory usage (if placed at end of gpioPinConfigs array).
 *  //
 *  GPIO_CallbackFxn gpioCallbackFunctions[] = {
 *      NULL,  //  Button 0
 *      NULL,  //  Button 1
 *  };
 *  @endcode
 *
 *  3.  The device specific GPIOCC26XX_Config structure that tells the GPIO
 *  driver where the two aforementioned arrays are and the number of elements
 *  in each. The interrupt priority of all pins configured to generate
 *  interrupts is also specified here. Values for the interrupt priority are
 *  device-specific. You should be well-acquainted with the interrupt
 *  controller used in your device before setting this parameter to a
 *  non-default value. The sentinel value of (~0) (the default value) is
 *  used to indicate that the lowest possible priority should be used.
 *  Below is an example of an initialized GPIOCC26XX_Config
 *  structure:
 *  @code
 *  const GPIOCC26XX_Config GPIOCC26XX_config = {
 *      .pinConfigs = (GPIO_PinConfig *)gpioPinConfigs,
 *      .callbacks = (GPIO_CallbackFxn *)gpioCallbackFunctions,
 *      .numberOfPinConfigs = sizeof(gpioPinConfigs)/sizeof(GPIO_PinConfig),
 *      .numberOfCallbacks = sizeof(gpioCallbackFunctions)/sizeof(GPIO_CallbackFxn),
 *      .intPriority = (~0)
 *  };
 *  @endcode
 *
 *  ============================================================================
 */

#ifndef TI_DRIVERS_GPIOCC26XX_CUSTOM_H
#define TI_DRIVERS_GPIOCC26XX_CUSTOM_H

#ifdef __cplusplus
extern "C" {
#endif

/*!
 *  @brief  GPIO device specific driver configuration structure
 *
 *  The device specific GPIOCC26XX_Config structure that tells the GPIO
 *  driver where the two aforementioned arrays are and the number of elements
 *  in each. The interrupt priority of all pins configured to generate
 *  interrupts is also specified here. Values for the interrupt priority are
 *  device-specific. You should be well-acquainted with the interrupt
 *  controller used in your device before setting this parameter to a
 *  non-default value. The sentinel value of (~0) (the default value) is
 *  used to indicate that the lowest possible priority should be used.
 *
 *  Below is an example of an initialized GPIOCC26XX_Config
 *  structure:
 *  @code
 *  const GPIOCC26XX_Config GPIOCC26XX_config = {
 *      .pinConfigs = (GPIO_PinConfig *)gpioPinConfigs,
 *      .callbacks = (GPIO_CallbackFxn *)gpioCallbackFunctions,
 *      .numberOfPinConfigs = sizeof(gpioPinConfigs)/sizeof(GPIO_PinConfig),
 *      .numberOfCallbacks = sizeof(gpioCallbackFunctions)/sizeof(GPIO_CallbackFxn),
 *      .intPriority = (~0)
 *  };
 *  @endcode
 */
#if !defined(GPIOCC26XX_Config)
typedef struct {
    /*! Pointer to the board's GPIO_PinConfig array */
    GPIO_PinConfig  *pinConfigs;

    /*! Pointer to the board's GPIO_CallbackFxn array */
    GPIO_CallbackFxn  *callbacks;

    /*! Number of GPIO_PinConfigs defined */
    uint32_t numberOfPinConfigs;

    /*! Number of GPIO_Callbacks defined */
    uint32_t numberOfCallbacks;

    /*!
     *  Interrupt priority used for call back interrupts.
     *
     *  intPriority is the interrupt priority, as defined by the
     *  underlying OS.  It is passed unmodified to the underlying OS's
     *  interrupt handler creation code, so you need to refer to the OS
     *  documentation for usage.  If the driver uses the ti.dpl
     *  interface instead of making OS calls directly, then the HwiP port
     *  handles the interrupt priority in an OS specific way.  In the case
     *  of the SYS/BIOS port, intPriority is passed unmodified to Hwi_create().
     *
     *  Setting ~0 will configure the lowest possible priority
     */
    uint32_t intPriority;
} GPIOCC26XX_Config;
#endif


#ifdef __cplusplus
}
#endif

#endif /* TI_DRIVERS_GPIOCC26XX_CUSTOM_H */
