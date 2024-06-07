#include <experimental/memory>

#include "config.h"
#include "rc/RetCode.h"

namespace config {
    /**
     * @struct IConfigManager
     * @brief  This interface manages the application's configuration settings. It can
     *         retrieve, apply, load, save, and restore the configuration.
     */
    struct IConfigManager {
        /**
         * @brief  Virtual destructor.
         */
        virtual ~IConfigManager() noexcept = default;

        /**
         * @brief   Getter for the current configuration.
         * @return  An instance of Config representing the current configuration.
         */
        [[nodiscard]] virtual const Config &getConfig() const noexcept = 0;

        /**
         * @brief  Applies the current configuration to the application.
         * @return An instance of Status indicating the success or failure of the application of the configuration.
         */
        virtual Status apply() noexcept = 0;

        /**
         * @brief  Applies a new configuration to the application.
         * @param  newConfig The new configuration that will be applied.
         * @return An instance of Status indicating the success or failure of the application of the configuration.
         */
        virtual Status apply(const Config &newConfig) noexcept = 0;

        /**
         * @brief  Loads the configuration from persistent storage like a file or database.
         * @return An instance of Status indicating the success or failure of the loading operation.
         */
        virtual Status load() noexcept = 0;

        /**
         * @brief  Saves the current configuration to persistent storage.
         * @return An instance of Status indicating the success or failure of the save operation.
         */
        virtual Status save() noexcept = 0;

        /**
         * @brief  Restores the configuration to its default state.
         * @return An instance of Status indicating the success or failure of the restore operation.
         */
        virtual Status restore() noexcept = 0;
    };

    using ConfigManagerObsPtr = std::experimental::observer_ptr<IConfigManager>;
}