//
// Created by hanna on 2017-11-09.
//

#pragma once
#include"../include/events.h"


FortifyEvent::FortifyEvent(){};

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

std::string FortifyEvent::printEvent()
{
    return this->armiesMoved + " armies are being moved from "
                         + this->source->getPointerToCountry()->getName()
                         + " to "+ this->destination->getPointerToCountry()->getName();
}