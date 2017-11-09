//
// Created by hanna on 2017-11-09.
//

#pragma once
#include"..include/events.h"


void FortifyEvent::FortifyEvent(){};

void FortifyEvent::setArmiesMoved(int armiesMoved)
{
    this.armiesMoved=armiesMoved;
}

void FortifyEvent::setSource(Node* source)
{
    this.source = source;
}

void FortifyEvent::setDestination(Node* destination)
{
    this.destination = destination;
}

int FortifyEvent::getArmiesMoved()
{
    return this.armiesMoved;
}

Node* FortifyEvent::getSource()
{
    return this.source;
}

Node* FortifyEvent::getDestination()
{
    return this.destination;
}