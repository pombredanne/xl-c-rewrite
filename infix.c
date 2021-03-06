// ****************************************************************************
//  infix.c                                          XL - An extensible language
// ****************************************************************************
//
//   File Description:
//
//     Representation of infix nodes, like A+B or A and B
//
//
//
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

#define INFIX_C
#include "infix.h"
#include "name.h"
#include "renderer.h"
#include <stdlib.h>
#include <strings.h>


tree_p infix_handler(tree_cmd_t cmd, tree_p tree, va_list va)
// ----------------------------------------------------------------------------
//   The common handler for prefix and postfix
// ----------------------------------------------------------------------------
{
    infix_p    infix = (infix_p) tree;
    renderer_p renderer;
    tree_p     left, right;
    name_p     opcode;

    switch(cmd)
    {
    case TREE_TYPENAME:
        // Return a default tree type name
        return (tree_p) "infix";

    case TREE_SIZE:
        // Return the size of the tree in bytes (is dynamic for infixs)
        return (tree_p) (sizeof(infix_t));

    case TREE_ARITY:
        // Infix nodes have three children
        return (tree_p) 3;

    case TREE_CAST:
        // Check if we cast to infix type, if so, success
        if (tree_cast_handler(va) == infix_handler)
            return tree;
        break;                      // Pass on to base class handler

    case TREE_CHILDREN:
        // Pointer to the children is right after the tree 'header
        return tree + 1;

    case TREE_INITIALIZE:
        // Fetch pointer to data and size from varargs list (see infix_new)
        opcode = va_arg(va, name_p);
        left = va_arg(va, tree_p);
        right = va_arg(va, tree_p);

        // Create infix and copy data in it
        infix = (infix_p) tree_malloc(sizeof(infix_t));
        infix->left = tree_use(left);
        infix->right = tree_use(right);
        infix->opcode = name_use(opcode);
        return (tree_p) infix;

    case TREE_RENDER:
        // Render left then right
        renderer = va_arg(va, renderer_p);
        render(renderer, (tree_p) infix->left);
        render(renderer, (tree_p) infix->opcode);
        render(renderer, (tree_p) infix->right);
        return tree;

    default:
        // Other cases are handled correctly by the tree handler
        break;
    }
    return tree_handler(cmd, tree, va);
}
