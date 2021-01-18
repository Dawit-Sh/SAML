/*
 * SAML 0.0.1
 * Author - Kidus W. (hello@kidusw.com)
 * License - GNU GPL v.3
 */

#include "Runtime.h"

SAML::Stack stack;

bool SAML::evaluate_ast(SAML::Tree& tree)
{
    /*ECHO Command*/

    if (tree.m_action == ACT_ECHO)
    {
        if (!(tree.m_children.size() <= 0)) {
            for (unsigned i = 0 ; i < tree.m_children.size(); i++)
            {
                SAML::Token _token = tree.m_children[i];
                std::cout << _token.m_value << " ";
            }
        }

        else {
            std::cout << std::endl;
        }

        return true;
    }

    /*VAR Command*/

    else if (tree.m_action == ACT_ALLOC_VAR)
    {
        if (tree.m_children.size() < 2)
        {
            std::cerr << "Error: Variable declaration not followed by variable data\n";
            return false;
        }

        Variable variable;
        variable.m_identifier = tree.m_children[0].m_value;
        variable.m_value = strtod(tree.m_children[1].m_value.c_str(), NULL);
        Array _array;
        _array.m_identifier = variable.m_identifier;

        if (!stack.var_exists(variable) && !stack.arr_exists(_array))
        {

            for (unsigned i = 0; i < tree.m_children[0].m_value.length(); i++)
            {
                if (isdigit(tree.m_children[0].m_value[i])) {
                    std::cerr << "Error: Illegal identifier name.\n";
                    return false;
                    break;
                }

                else {
                    continue;
                }
            }

            if (tree.m_children[0].m_tkclass == SAML::TK_ILLEGAL)
            {
                std::cerr << "Error: Illegal identifier name.\n";
                return false;
            }

            else if (!(tree.m_children[1].m_tkclass == SAML::TK_NUMVALUE))
            {
                std::cerr << "Error: Non-numeric value assigned to VAR object\n";
                return false;
            }

            else if (tree.m_children.size() > 2)
            {
                std::cerr << "Warning: Truncating extra data off variable declaration.\n";
                stack.add(variable);
                return false;
            }

            stack.add(variable);
            return true;
        }

        else if (stack.var_exists(variable))
        {
            Variable var = stack.retrieve_var(variable.m_identifier);
            std::cerr << "Error: Variable '" << var.m_identifier << "' already declared with value " << var.m_value << "\n";
            return false;
        }

        else if (stack.arr_exists(_array))
        {
            std::cerr << "Error: Array exists under same identifier\n";
            return false;
        }
    }

    /*ARRAY Command*/

    else if (tree.m_action == ACT_ALLOC_ARRAY)
    {
        if (tree.m_children.size() < 2)
        {
            std::cerr << "Error: Array declaration not followed by array data\n";
            return false;
        }

        Array _array;
        _array.m_identifier = tree.m_children[0].m_value;
        Variable variable;
        variable.m_identifier = tree.m_children[0].m_value;

        if (!stack.var_exists(variable) && !stack.arr_exists(_array))
        {
            if (tree.m_children[0].m_tkclass == SAML::TK_ILLEGAL)
            {
                std::cerr << "Error: Illegal identifier name.\n";
                return false;
            }

            for (unsigned i = 0; i < tree.m_children[0].m_value.length(); i++)
            {
                if (isdigit(tree.m_children[0].m_value[i])) {
                    std::cerr << "Error: Illegal identifier name.\n";
                    return false;
                    break;
                }

                else {
                    continue;
                }
            }

            for (unsigned i = 0; i < tree.m_children.size(); i++)
            {
                if (tree.m_children[i].m_tkclass == TK_ILLEGAL)
                {
                    std::cerr << "Error: Non-numeric value given in array value initialisation list\n";
                    return false;
                    break;
                }

                else if (tree.m_children[i].m_tkclass == TK_NULL)
                {
                    continue;
                }

                else
                {
                    _array.m_value.push_back(strtod(tree.m_children[i].m_value.c_str(), NULL));
                    continue;
                };
            }

            stack.add(_array);
            return true;
        }

        else if (stack.arr_exists(_array))
        {
            std::cerr << "Error: Array '" << _array.m_identifier << "' already declared.\n";
            return false;
        }

        else if (stack.var_exists(variable))
        {
            std::cerr << "Error: Variable exists under same identifier\n";
            return false;
        }
    }

    /*PRINT Command*/

    else if (tree.m_action == ACT_OUT)
    {
        if (tree.m_children.size() > 1)
        {
            std::cerr << "Error: More than one variable set as parameter for PRINT function\n";
            return false;
        }

        else if (tree.m_children.size() <= 0)
        {
            std::cerr << "Error: PRINT what?\n";
            return false;
        }

        else
        {
            Variable* _var = new Variable;
            Array* _arr = new Array;

            _var->m_identifier = tree.m_children[0].m_value;
            _arr->m_identifier = tree.m_children[0].m_value;

            if (stack.var_exists((*_var)))
            {
                Variable var = stack.retrieve_var(_var->m_identifier);
                std::cout << var.m_value;
                return true;
            }

            else if (stack.arr_exists((*_arr)))
            {
                Array arr = stack.retrieve_array(_arr->m_identifier);

                std::cout << std::endl;

                for (unsigned i = 0; i < arr.m_value.size(); i++)
                {
                    if (arr.m_value[i] != 0) {
                        std::cout << arr.m_value[i] << "\n";
                    }

                    else {
                        continue;
                    }
                }

                return true;
            }

            else if (!(stack.var_exists((*_var))) || !(stack.arr_exists((*_arr))))
            {
                std::cerr << "Error: Object '" << _var->m_identifier << "' undeclared.\n";
                return false;
            }

            delete _var;
            delete _arr;
        }
    }

    /*GET Command*/

    else if (tree.m_action == ACT_IN)
    {
        if (tree.m_children.size() > 1)
        {
            std::cout << "Error: More than one variable/array specified for GET function.\n";
            return false;
        }

        else if (tree.m_children.size() <= 0)
        {
            std::cout << "Error: GET what?\n";
            return false;
        }

        else {
            Variable _var;
            Array _arr;

            _var.m_identifier = tree.m_children[0].m_value;
            _arr.m_identifier = tree.m_children[0].m_value;

            if (!stack.arr_exists(_arr) && !stack.var_exists(_var))
            {
                std::cerr << "Object '"<< _var.m_identifier << "' undeclared.\n";
                return false;
            }

            else if (stack.arr_exists(_arr)) {
                Array arr = stack.retrieve_array(_arr.m_identifier);
                double values;

                do {
                    std::cin >> values;
                    arr.m_value.push_back(values);
                } while (values != 0);

                stack.add(arr);
                return true;
            }

            else if (stack.var_exists(_var)) {
                Variable var = stack.retrieve_var(_var.m_identifier);
                double value;
                std::cin >> value;
                var.m_value = value;
                stack.add(var);
                return true;
            }
        }
    }

    /*PUSH Command*/

    else if (tree.m_action == ACT_PUSH)
    {
        if (tree.m_children.size() < 2)
        {
            std::cerr << "Error: Not enough parameters for function PUSH.\n";
            return false;
        }

        else if (tree.m_children.size() > 2)
        {
            std::cerr << "Error: Too many parameters for function PUSH\n";
        }

        else {
            Variable _var;
            Array _arr;

            _var.m_identifier = tree.m_children[0].m_value;
            _arr.m_identifier = tree.m_children[0].m_value;

            double pushable = strtof(tree.m_children[1].m_value.c_str(), NULL);

            if (!stack.var_exists(_var) && !stack.arr_exists(_arr))
            {
                std::cerr << "Error: Object " << _var.m_identifier << " undeclared.\n";
                return false;
            }

            else if (stack.var_exists(_var))
            {
                _var.m_value = pushable;
                stack.add(_var);
                return true;
            }

            else if (stack.arr_exists(_arr))
            {
                _arr.m_value.push_back(pushable);
                stack.add(_arr);
                return true;
            }
        }
    }

    /*TOTAL Command*/

    else if (tree.m_action == ACT_OP_TOTAL)
    {
        if (tree.m_children.size() <= 0)
        {
            std::cerr << "Error: Function TOTAL called without parameter\n";
            return false;
        }

        else if (tree.m_children.size() > 1)
        {
            std::cerr << "Error: Function TOTAL accepts only one parameter\n";
            return false;
        }

        else {
            Array _arr; Variable _var;
            _arr.m_identifier = tree.m_children[0].m_value;
            _var.m_identifier = tree.m_children[0].m_value;

            if (stack.var_exists(_var))
            {
                std::cerr << "Error: Parameter '" << _var.m_identifier << "' supplied is already declared as variable.\n";
                return false;
            }

            else if (!stack.arr_exists(_arr))
            {
                std::cerr << "Array '" << _arr.m_identifier << "' undeclared.\n";
                return false;
            }

            else {
                Array arr = stack.retrieve_array(_arr.m_identifier);
                double total = SAML::find_total(arr.m_value);
                std::cout << total;
            }
        }
    }

    /*MEAN Command*/

    else if (tree.m_action == ACT_OP_MEAN)
    {
        if (tree.m_children.size() <= 0)
        {
            std::cerr << "Error: Function MEAN called without parameter\n";
            return false;
        }

        else if (tree.m_children.size() > 1)
        {
            std::cerr << "Error: Function MEAN accepts only one parameter\n";
            return false;
        }

        else {
            Array _arr; Variable _var;
            _arr.m_identifier = tree.m_children[0].m_value;
            _var.m_identifier = tree.m_children[0].m_value;

            if (stack.var_exists(_var))
            {
                std::cerr << "Error: Parameter '" << _var.m_identifier << "' supplied is already declared as variable.\n";
                return false;
            }

            else if (!stack.arr_exists(_arr))
            {
                std::cerr << "Array '" << _arr.m_identifier << "' undeclared.\n";
                return false;
            }

            else {
                Array arr = stack.retrieve_array(_arr.m_identifier);
                double mean = SAML::find_mean(arr.m_value);
                std::cout << mean;
            }
        }
    }

    /*MODE Command*/

    else if (tree.m_action == ACT_OP_MODE)
    {
        if (tree.m_children.size() <= 0)
        {
            std::cerr << "Error: Function MODE called without parameter\n";
            return false;
        }

        else if (tree.m_children.size() > 1)
        {
            std::cerr << "Error: Function MODE accepts only one parameter\n";
            return false;
        }

        else {
            Array _arr; Variable _var;
            _arr.m_identifier = tree.m_children[0].m_value;
            _var.m_identifier = tree.m_children[0].m_value;

            if (stack.var_exists(_var))
            {
                std::cerr << "Error: Parameter '" << _var.m_identifier << "' supplied is already declared as variable.\n";
                return false;
            }

            else if (!stack.arr_exists(_arr))
            {
                std::cerr << "Array '" << _arr.m_identifier << "' undeclared.\n";
                return false;
            }

            else {
                Array arr = stack.retrieve_array(_arr.m_identifier);
                double mode = SAML::find_mode(arr.m_value);
                std::cout << mode;
            }
        }
    }

    /*MEDIAN Command*/

    else if (tree.m_action == ACT_OP_MEDIAN)
    {
        if (tree.m_children.size() <= 0)
        {
            std::cerr << "Error: Function MEDIAN called without parameter\n";
            return false;
        }

        else if (tree.m_children.size() > 1)
        {
            std::cerr << "Error: Function MEDIAN accepts only one parameter\n";
            return false;
        }

        else {
            Array _arr; Variable _var;
            _arr.m_identifier = tree.m_children[0].m_value;
            _var.m_identifier = tree.m_children[0].m_value;

            if (stack.var_exists(_var))
            {
                std::cerr << "Error: Parameter '" << _var.m_identifier << "' supplied is already declared as variable.\n";
                return false;
            }

            else if (!stack.arr_exists(_arr))
            {
                std::cerr << "Array '" << _arr.m_identifier << "' undeclared.\n";
                return false;
            }

            else {
                Array arr = stack.retrieve_array(_arr.m_identifier);
                double median = SAML::find_median(arr.m_value);
                std::cout << median;
            }
        }
    }

    /*DEVIATE Command*/

    else if (tree.m_action == ACT_OP_DEVIATION)
    {
        if (tree.m_children.size() <= 0)
        {
            std::cerr << "Error: Function DEVIATE called without parameter\n";
            return false;
        }

        else if (tree.m_children.size() > 1)
        {
            std::cerr << "Error: Function DEVIATE accepts only one parameter\n";
            return false;
        }

        else {
            Array _arr; Variable _var;
            _arr.m_identifier = tree.m_children[0].m_value;
            _var.m_identifier = tree.m_children[0].m_value;

            if (stack.var_exists(_var))
            {
                std::cerr << "Error: Parameter '" << _var.m_identifier << "' supplied is already declared as variable.\n";
                return false;
            }

            else if (!stack.arr_exists(_arr))
            {
                std::cerr << "Array '" << _arr.m_identifier << "' undeclared.\n";
                return false;
            }

            else {
                Array arr = stack.retrieve_array(_arr.m_identifier);
                unsigned deviance = SAML::deviate_list(arr.m_value);
                std::cout << deviance;
            }
        }
    }

    /*VARIANCE Command*/
    else if (tree.m_action == ACT_OP_VARIANCE)
    {
        if (tree.m_children.size() <= 0)
        {
            std::cerr << "Error: Function VARIANCE called without parameter\n";
            return false;
        }

        else if (tree.m_children.size() > 1)
        {
            std::cerr << "Error: Function VARIANCE accepts only one parameter\n";
            return false;
        }

        else {
            Array _arr; Variable _var;
            _arr.m_identifier = tree.m_children[0].m_value;
            _var.m_identifier = tree.m_children[0].m_value;

            if (stack.var_exists(_var))
            {
                std::cerr << "Error: Parameter '" << _var.m_identifier << "' supplied is already declared as variable.\n";
                return false;
            }

            else if (!stack.arr_exists(_arr))
            {
                std::cerr << "Array '" << _arr.m_identifier << "' undeclared.\n";
                return false;
            }

            else {
                Array arr = stack.retrieve_array(_arr.m_identifier);
                double variance = SAML::find_variance(arr.m_value);
                std::cout << variance;
            }
        }
    }

    /*RANGE Command*/

    else if (tree.m_action == ACT_OP_RANGE)
    {
        if (tree.m_children.size() <= 0)
        {
            std::cerr << "Error: Function RANGE called without parameter\n";
            return false;
        }

        else if (tree.m_children.size() > 1)
        {
            std::cerr << "Error: Function RANGE accepts only one parameter\n";
            return false;
        }

        else {
            Array _arr; Variable _var;
            _arr.m_identifier = tree.m_children[0].m_value;
            _var.m_identifier = tree.m_children[0].m_value;

            if (stack.var_exists(_var))
            {
                std::cerr << "Error: Parameter '" << _var.m_identifier << "' supplied is already declared as variable.\n";
                return false;
            }

            else if (!stack.arr_exists(_arr))
            {
                std::cerr << "Array '" << _arr.m_identifier << "' undeclared.\n";
                return false;
            }

            else {
                Array arr = stack.retrieve_array(_arr.m_identifier);
                double range = SAML::find_range(arr.m_value);
                std::cout << range;
            }
        }
    }

    /*No main action supplied...*/

    else if (tree.m_action == ACT_ERROR)
    {
        std::cerr << "Error: Unknown operand specified.\n";
        return false;
    }

    return true;
}
