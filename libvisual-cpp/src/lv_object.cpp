// Libvisual-c++ - C++ bindings for Libvisual
// 
// Copyright (C) 2005 Chong Kai Xiong <descender@phreaker.net>
//
// Author: Chong Kai Xiong <descender@phreaker.net>
//
// $Id: lv_object.cpp,v 1.3 2005-09-01 07:10:25 descender Exp $
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as
// published by the Free Software Foundation; either version 2.1
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA

#include <lv_config.hpp>
#include <lv_object.hpp>
#include <cstdlib>


#ifdef LVCPP_OBJECT_TEST

#include <libvisual_cpp.hpp>
#include <iostream>

int test_visual_object_dtor (VisObject *object)
{
    char *privates = static_cast<char *> (visual_object_get_private (object));

    std::cout << "Stored private data: " << privates << "\n";

    return VISUAL_OK;
}

int main (int argc, char **argv)
{
    Lv::init (argc, argv);

    std::cout << "Lv::Object test\n";

    {
        Lv::Object object;

        for (int i = 0; i < 5; i++)
            object.ref ();

        for (int i = 0; i < 5; i++)
            object.unref ();

        // Libvisual doesn't provide a function to get the reference count,        
        // so we peek inside ourselves
        std::cout << "Final reference count: " << object.vis_object ().refcount << "\n";
    }

    {
        Lv::Object a;

        visual_object_set_dtor (&a.vis_object (), test_visual_object_dtor);
        visual_object_set_private (&a.vis_object (), const_cast<void *> (static_cast<const void *> ("hello world!")));
    }

    {
        Lv::Object a;

        visual_object_set_dtor (&a.vis_object (), test_visual_object_dtor);
        visual_object_set_private (&a.vis_object (), const_cast<void *> (static_cast<const void *> ("hello world again!")));

        Lv::Object b(a);
    }

    Lv::quit ();

    return 0;
}

#endif // #ifdef LV_CPP_OBJECT_TEST