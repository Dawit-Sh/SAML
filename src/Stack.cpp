/*
 * SAML 0.0.1
 * Author - Kidus W. (hello@kidusw.com)
 * License - GNU GPL v.3
 */

#include "Stack.h"

void SAML::Stack::add(Variable& var)
{
    if (!this->var_exists(var)) {
        this->m_varstack.push_back(var);
    }

    else {
        for (unsigned i = 0; i < this->m_varstack.size(); i++)
        {
            if (var.comp(this->m_varstack[i]))
            {
                this->m_varstack[i].m_identifier = var.m_identifier;
                this->m_varstack[i].m_value = var.m_value;
                break;
            }

            else {
                continue;
            }
        }
    }
}

void SAML::Stack::add(Array& arr)
{
    if (!this->arr_exists(arr)) {
        this->m_arrstack.push_back(arr);
    }

    else {
        for (unsigned i = 0; i < this->m_arrstack.size(); i++)
        {
            if (arr.comp(this->m_arrstack[i]))
            {
                this->m_arrstack[i].m_identifier = arr.m_identifier;
                this->m_arrstack[i].m_value = arr.m_value;
                break;
            }

            else {
                continue;
            }
        }
    }
}

SAML::Variable SAML::Stack::retrieve_var(std::string& identifier)
{
    SAML::Variable _var;

    for (unsigned i = 0; i < this->m_varstack.size(); i++)
    {
        if (this->m_varstack[i].m_identifier == identifier)
        {
            _var.m_identifier = identifier;
            _var.m_value = this->m_varstack[i].m_value;
            break;
        }

        else {
            continue;
        }
    }

    return _var;
}

SAML::Array SAML::Stack::retrieve_array(std::string& identifier)
{
    SAML::Array _arr;

    for (unsigned i = 0; i < this->m_arrstack.size(); i++)
    {
        if (this->m_arrstack[i].m_identifier == identifier)
        {
            _arr.m_identifier = identifier;
            _arr.m_value = this->m_arrstack[i].m_value;
            break;
        }

        else {
            continue;
        }
    }

    return _arr;
}

bool SAML::Stack::var_exists(SAML::Variable& variable)
{
    bool ret_val = false;

    for (unsigned i = 0; i < this->m_varstack.size(); i++)
    {
        if (this->m_varstack[i].comp(variable))
        {
            ret_val = true;
            break;

        }

        else {
            continue;
        }
    }

    return ret_val;
}

bool SAML::Stack::arr_exists(SAML::Array& array)
{
    bool ret_val = false;

    for (unsigned i = 0; i < this->m_arrstack.size(); i++)
    {
        if (this->m_arrstack[i].comp(array))
        {
            ret_val = true;
            break;
        }

        else {
            continue;
        }
    }

    return ret_val;
}

bool SAML::Variable::comp(SAML::Variable& _var)
{
    return (this->m_identifier == _var.m_identifier);
}

bool SAML::Array::comp(SAML::Array& _arr)
{
    return (this->m_identifier == _arr.m_identifier);
}


