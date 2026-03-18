#include "WCMS.h"

int main(void)
{
    t_sh    sh;
    t_env   pwd;
    t_env   *result;

    ft_bzero(&sh, sizeof(t_sh));
    ft_bzero(&pwd, sizeof(t_env));
    sh.gc = gc_checkpoint(&sh);
    pwd.exported = 1;
    pwd.has_value = 1;
    pwd.next = NULL;

    env_addback(&sh, env_newnode(&sh, "PWD", "/home/kedemiro/Desktop/Examrank03/filter"));

    // Test 1
    pwd.env_value = "/home/kedemiro/Desktop/Examrank03/filter";
    logical_pwd_update(&sh, "test/source", &pwd);
    result = find_env(&sh, "PWD");
    ft_printf("Test1  ../\n");
    ft_printf("  cikti   : %s\n", result ? result->env_value : "NULL");
    ft_printf("  beklenen: /home/kedemiro/Desktop/Examrank03/filter/test/source\n\n");

    // Test 2
    pwd.env_value = "/home/kedemiro/Desktop/Examrank03/filter";
    logical_pwd_update(&sh, "../../", &pwd);
    result = find_env(&sh, "PWD");
    ft_printf("Test2  ../../\n");
    ft_printf("  cikti   : %s\n", result ? result->env_value : "NULL");
    ft_printf("  beklenen: /home/kedemiro/Desktop\n\n");

    // Test 3
    pwd.env_value = "/home/kedemiro/Desktop/Examrank03/filter";
    logical_pwd_update(&sh, "./", &pwd);
    result = find_env(&sh, "PWD");
    ft_printf("Test3  ./\n");
    ft_printf("  cikti   : %s\n", result ? result->env_value : "NULL");
    ft_printf("  beklenen: /home/kedemiro/Desktop/Examrank03/filter\n\n");

    // Test 4
    pwd.env_value = "/home/kedemiro/Desktop/Examrank03/filter";
    logical_pwd_update(&sh, "/tmp/test", &pwd);
    result = find_env(&sh, "PWD");
    ft_printf("Test4  /tmp/test\n");
    ft_printf("  cikti   : %s\n", result ? result->env_value : "NULL");
    ft_printf("  beklenen: /tmp/test\n\n");

    // Test 5
    pwd.env_value = "/home/kedemiro/Desktop/Examrank03/filter";
    logical_pwd_update(&sh, "../../../../../../../../../", &pwd);
    result = find_env(&sh, "PWD");
    ft_printf("Test5  fazla ../\n");
    ft_printf("  cikti   : %s\n", result ? result->env_value : "NULL");
    ft_printf("  beklenen: /\n\n");

    // Test 6
    pwd.env_value = "/home/kedemiro/Desktop/Examrank03/filter";
    logical_pwd_update(&sh, "../../minishell/./src/../includes/libft", &pwd);
    result = find_env(&sh, "PWD");
    ft_printf("Test6  ../../minishell/./src/../includes/libft\n");
    ft_printf("  cikti   : %s\n", result ? result->env_value : "NULL");
    ft_printf("  beklenen: /home/kedemiro/Desktop/minishell/includes/libft\n\n");

    // Edge 1
    pwd.env_value = "/";
    logical_pwd_update(&sh, "./", &pwd);
    result = find_env(&sh, "PWD");
    ft_printf("Edge1  root/.\n");
    ft_printf("  cikti   : %s\n", result ? result->env_value : "NULL");
    ft_printf("  beklenen: /\n\n");

    // Edge 2
    pwd.env_value = "/";
    logical_pwd_update(&sh, "../", &pwd);
    result = find_env(&sh, "PWD");
    ft_printf("Edge2  root/..\n");
    ft_printf("  cikti   : %s\n", result ? result->env_value : "NULL");
    ft_printf("  beklenen: /\n\n");

    // Edge 3
    pwd.env_value = "/home";
    logical_pwd_update(&sh, "../", &pwd);
    result = find_env(&sh, "PWD");
    ft_printf("Edge3  /home/..\n");
    ft_printf("  cikti   : %s\n", result ? result->env_value : "NULL");
    ft_printf("  beklenen: /\n\n");

    // Edge 4
    pwd.env_value = "/home/kedemiro";
    logical_pwd_update(&sh, "././././", &pwd);
    result = find_env(&sh, "PWD");
    ft_printf("Edge4  ././././\n");
    ft_printf("  cikti   : %s\n", result ? result->env_value : "NULL");
    ft_printf("  beklenen: /home/kedemiro\n\n");

    // Edge 5
    pwd.env_value = "/home/kedemiro/a/b/c";
    logical_pwd_update(&sh, "../../../", &pwd);
    result = find_env(&sh, "PWD");
    ft_printf("Edge5  ../../../\n");
    ft_printf("  cikti   : %s\n", result ? result->env_value : "NULL");
    ft_printf("  beklenen: /home/kedemiro\n\n");

    // Edge 6
    pwd.env_value = "/home/kedemiro";
    logical_pwd_update(&sh, "./.././.././", &pwd);
    result = find_env(&sh, "PWD");
    ft_printf("Edge6  ./.././.././\n");
    ft_printf("  cikti   : %s\n", result ? result->env_value : "NULL");
    ft_printf("  beklenen: /\n\n");

    // Edge 7
    pwd.env_value = "/home/kedemiro";
    logical_pwd_update(&sh, "../test", &pwd);
    result = find_env(&sh, "PWD");
    ft_printf("Edge7  ../test\n");
    ft_printf("  cikti   : %s\n", result ? result->env_value : "NULL");
    ft_printf("  beklenen: /home/test\n\n");

    // Edge 8
    pwd.env_value = "/home/kedemiro";
    logical_pwd_update(&sh, "/", &pwd);
    result = find_env(&sh, "PWD");
    ft_printf("Edge8  abs /\n");
    ft_printf("  cikti   : %s\n", result ? result->env_value : "NULL");
    ft_printf("  beklenen: /\n\n");

    gc_free_all(&sh);
    return (0);
}