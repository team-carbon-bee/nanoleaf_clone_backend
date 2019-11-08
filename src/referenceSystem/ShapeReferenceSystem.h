#pragma once

#include <Arduino.h>
#include <ArduinoJson.h>

#include <LinkedList.h>

#include "IReferenceSystem.h"

#include "ShapeObjects.h"
#include "ShapeHelper.h"
#include "PixelHelper.h"
#include "ShapeDetails.h"
#include "ConfigurationProvider.h"
#include "ledDriver/ILedDriver.h"

namespace referenceSystem {

class ShapeReferenceSystem : public IReferenceSystem
{
    public:
        ShapeReferenceSystem();
        virtual ~ShapeReferenceSystem();

        void setup(ledDriver::ILedDriver * ledDriver);
        void driveLeds();
        
        //cached value for performance reasons
        int shapeCount();
        void clear();
        void clear(Shape * node);
        void fill(const Color c);
        void fill(Shape * node, const Color c);

        //Real assembly of the shapes
        Shape * assembly();

        //Linked List representation
        LinkedList<Shape *> & shapeList();

        static ShapeDetails * getShape(Shape * node);
        void clearAnimationObject();

        //Return random shape of assembly 
        Shape * getRandomShape();
        //Return random child shape of node 
        Shape * getRandomShape(Shape * node);
        //Return random shape of assembly except provided one
        Shape * getRandomShapeExcept(Shape * notThisOne);
        //Return random child shape of node except provided one
        Shape * getRandomShapeExcept(Shape * node, Shape * notThisOne);
    private:
        ledDriver::ILedDriver * _ledDriver;
        Shape * _assembly;
        int _shapeCount;
        LinkedList<Shape *> _shapeList;

        void createShapeDetailObjects(Shape * shape);
        void createLinkedListFromShapes(Shape * shape);
        int prepareDriveLeds(Shape * node, const int offset = 0);
};

#if !defined(NO_GLOBAL_INSTANCES)
extern ShapeReferenceSystem ShapeRef;
#endif

} //referenceSystem