// ****************************************************************************
//  pfix.c                                          XL - An extensible language
// ****************************************************************************
//
//   File Description:
//
//     Representation of prefix and postfix nodes
//     Prefix nodes represent +A or sin A
//     Postfix nodes represent A% or A km
//
//
//
//
//
// ****************************************************************************
//  (C) 2017 Christophe de Dinechin <christophe@dinechin.org>
//   This software is licensed under the GNU General Public License v3
//   See LICENSE file for details.
// ****************************************************************************

#define PFIX_C
#include "pfix.h"
#include "renderer.h"
#include <stdlib.h>
#include <strings.h>


tree_p pfix_handler(tree_cmd_t cmd, tree_p tree, va_list va)
// ----------------------------------------------------------------------------
//   The common handler for prefix and postfix
// ----------------------------------------------------------------------------
{
    pfix_p     pfix = (pfix_p) tree;
    renderer_p renderer;
    tree_p     left, right;

    switch(cmd)
    {
    case TREE_TYPENAME:
        // Return a default tree type name
        return (tree_p) "pfix";

    case TREE_SIZE:
        // Return the size of the tree in bytes (is dynamic for pfixs)
        return (tree_p) (sizeof(pfix_t));

    case TREE_ARITY:
        // Prefix and postfix have two children
        return (tree_p) 2;

    case TREE_CAST:
        // Check if we cast to blob type, if so, success
        if (tree_cast_handler(va) == pfix_handler)
            return tree;
        break;                      // Pass on to base class handler

    case TREE_CHILDREN:
        // Pointer to the children is right after the tree 'header
        return tree + 1;

    case TREE_INITIALIZE:
        // Fetch pointer to data and size from varargs list (see pfix_new)
        left = va_arg(va, tree_p);
        right = va_arg(va, tree_p);

        // Create pfix and copy data in it
        pfix = (pfix_p) tree_malloc(sizeof(pfix_t));
        pfix->left = tree_use(left);
        pfix->right = tree_use(right);
        return (tree_p) pfix;

    case TREE_RENDER:
        // Render left then right
        renderer = va_arg(va, renderer_p);
        render(renderer, pfix->left);
        render(renderer, pfix->right);
        return tree;

    default:
        break;
    }
    // Other cases are handled correctly by the tree handler
    return tree_handler(cmd, tree, va);
}


tree_p prefix_handler(tree_cmd_t cmd, tree_p tree, va_list va)
// ----------------------------------------------------------------------------
//   The prefix handler
// ----------------------------------------------------------------------------
{
    switch(cmd)
    {
    case TREE_TYPENAME:
        // Return a default tree type name
        return (tree_p) "prefix";

    case TREE_CAST:
        // Check if we cast to blob type, if so, success
        if (tree_cast_handler(va) == prefix_handler)
            return tree;
        break;                      // Pass on to base class handler

    default:
        break;
    }
    // Other cases are handled correctly by the tree handler
    return pfix_handler(cmd, tree, va);
}


tree_p postfix_handler(tree_cmd_t cmd, tree_p tree, va_list va)
// ----------------------------------------------------------------------------
//   The prefix handler
// ----------------------------------------------------------------------------
{
    switch(cmd)
    {
    case TREE_TYPENAME:
        // Return a default tree type name
        return (tree_p) "postfix";

    case TREE_CAST:
        // Check if we cast to blob type, if so, success
        if (tree_cast_handler(va) == postfix_handler)
            return tree;
        break;                      // Pass on to base class handler

    default:
        // Other cases are handled correctly by the tree handler
        break;
    }
    return pfix_handler(cmd, tree, va);
}
