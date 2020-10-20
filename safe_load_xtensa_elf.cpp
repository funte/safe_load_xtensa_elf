#include <pro.h>
#include <idp.hpp>
#include <diskio.hpp>
#include <loader.hpp>


// ELF machine type : Tensilica Xtensa Architecture.
int EM_XTENSA = 94;

/* Callback for ELF loader machine type checkpoint. */
static ssize_t idaapi idp_callback(void * /*user_data*/, 
    int notification_code, 
    va_list va)
{
    switch (notification_code)
    {
    case processor_t::ev_loader_elf_machine:
    {
        /*linput_t *li =*/ va_arg(va, linput_t *);
        int machine_type = va_arg(va, int);

        // Intercept ELF binary with Tensilica Xtensa Architecture.
        if (machine_type == EM_XTENSA)
        {
            const char ** p_procname = va_arg(va, const char **);

            // Set the Xtensa processor moudle(in file <IDADIR>/procs/xtensa70.py) to disassemble the current binary,
            // that has its `psnames` attribute set to "xtensa".
            static const char *my_proc_name = "xtensa";

            *p_procname = my_proc_name;
            msg("safe_load_xten_object: Intercepted ev_loader_elf_machine for xtensa.\n");

            return machine_type;
        }

        break;
    }
    default:
        break;
    }

    return 0;
}

int idaapi init(void) 
{
    hook_to_notification_point(HT_IDP, idp_callback, NULL);
    return PLUGIN_KEEP;
}

void idaapi term(void) 
{
    unhook_from_notification_point(HT_IDP, idp_callback, NULL);
}

bool idaapi run(size_t /*arg*/)
{
    warning("hello runing");
    return true;
}

const char *comment = "Avoid IDA70 warning \"Undefined or unknown machine type \
    94\" from loading Xtensa architecture ELF binary";
const char *wanted_name = "Safe load Xtensa ELF";

/* PLUGIN DESCRIPTION BLOCK */
extern "C" plugin_t PLUGIN =
{
    IDP_INTERFACE_VERSION,
    PLUGIN_FIX,             // Plugin flags.
    init,                   // Initialize.
    term,                   // Terminate.
    run,                    // Invoke plugin.
    comment,                // Statusline text.
    nullptr,                // Multi-line help about the plugin, unused.
    wanted_name,            // Preferred short name of the plugin.
    nullptr                 // Preferred hotkey to run the plugin, unused.
};