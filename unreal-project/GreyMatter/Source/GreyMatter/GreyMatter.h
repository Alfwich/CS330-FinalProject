// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#ifndef __GREYMATTER_H__
#define __GREYMATTER_H__

#include "CoreUObject.h"
#include "Engine.h"

//TEMPLATE Load Obj From Path
template <typename ObjClass>
static FORCEINLINE ObjClass* LoadObjFromPath(const FName& Path) {
	return Cast<ObjClass>(StaticLoadObject( ObjClass::StaticClass(), NULL,*Path.ToString()));
}

#endif
