//**************************************************************************/
// Copyright (c) 1998-2007 Autodesk, Inc.
// All rights reserved.
// 
// These coded instructions, statements, and computer programs contain
// unpublished proprietary information written by Autodesk, Inc., and are
// protected by Federal copyright law. They may not be disclosed to third
// parties or copied or duplicated in any form, in whole or in part, without
// the prior written consent of Autodesk, Inc.
//**************************************************************************/
// DESCRIPTION: Appwizard generated plugin
// AUTHOR: 
//***************************************************************************/

#include "maxPlugin.h"

#define maxPlugin_CLASS_ID	Class_ID(0x720a0d91, 0x68b0d82b)


#define PBLOCK_REF	0


class maxPlugin : public Modifier 
{
public:
	//Constructor/Destructor
	maxPlugin();
	virtual ~maxPlugin();

	virtual void DeleteThis() { delete this; }

	// From Animatable
	virtual const TCHAR *GetObjectName() { return GetString(IDS_CLASS_NAME); }

		virtual ChannelMask ChannelsUsed()  { return GEOM_CHANNEL|TOPO_CHANNEL; }
	#pragma message(TODO("Add the channels that the modifier actually modifies"))
	virtual ChannelMask ChannelsChanged() { return GEOM_CHANNEL; }
		
	#pragma message(TODO("Return the ClassID of the object that the modifier can modify"))
	Class_ID InputType() {return defObjectClassID;}

	virtual void ModifyObject(TimeValue t, ModContext &mc, ObjectState *os, INode *node);
	virtual void NotifyInputChanged(const Interval& changeInt, PartID partID, RefMessage message, ModContext *mc);

	virtual void NotifyPreCollapse(INode *node, IDerivedObject *derObj, int index);
	virtual void NotifyPostCollapse(INode *node,Object *obj, IDerivedObject *derObj, int index);


	virtual Interval LocalValidity(TimeValue t);

	// From BaseObject
	//#pragma message(TODO("Return true if the modifier changes topology"))
	virtual BOOL ChangeTopology() {return FALSE;}
	
	virtual CreateMouseCallBack* GetCreateMouseCallBack() {return NULL;}

	virtual BOOL HasUVW();
	virtual void SetGenUVW(BOOL sw);


	virtual void BeginEditParams(IObjParam *ip, ULONG flags,Animatable *prev);
	virtual void EndEditParams(IObjParam *ip, ULONG flags,Animatable *next);


	virtual Interval GetValidity(TimeValue t);

	// Automatic texture support
	
	// Loading/Saving
	virtual IOResult Load(ILoad *iload);
	virtual IOResult Save(ISave *isave);

	//From Animatable
	virtual Class_ID ClassID() {return maxPlugin_CLASS_ID;}
	virtual SClass_ID SuperClassID() { return OSM_CLASS_ID; }
	virtual void GetClassName(TSTR& s) {s = GetString(IDS_CLASS_NAME);}

	virtual RefTargetHandle Clone( RemapDir &remap );
	virtual RefResult NotifyRefChanged(const Interval& changeInt, RefTargetHandle hTarget, PartID& partID, RefMessage message, BOOL propagate);

	virtual int NumSubs() { return 1; }
	virtual TSTR SubAnimName(int /*i*/) { return GetString(IDS_PARAMS); }
	virtual Animatable* SubAnim(int /*i*/) { return pblock; }

	// TODO: Maintain the number or references here
	virtual int NumRefs() { return 1; }
	virtual RefTargetHandle GetReference(int i);

	virtual int	NumParamBlocks() { return 1; }					// return number of ParamBlocks in this instance
	virtual IParamBlock2* GetParamBlock(int /*i*/) { return pblock; } // return i'th ParamBlock
	virtual IParamBlock2* GetParamBlockByID(BlockID id) { return (pblock->ID() == id) ? pblock : NULL; } // return id'd ParamBlock

protected:
	virtual void SetReference(int , RefTargetHandle rtarg);

private:
	// Parameter block
	IParamBlock2 *pblock; //ref 0
};

void maxPlugin::SetReference( int i, RefTargetHandle rtarg )
{
	if (i == PBLOCK_REF)
	{
		pblock=(IParamBlock2*)rtarg;
	}
}

RefTargetHandle maxPlugin::GetReference( int i)
{
	if (i == PBLOCK_REF)
	{
		return pblock;
	}
	return nullptr;
}



class maxPluginClassDesc : public ClassDesc2 
{
public:
	virtual int IsPublic() 							{ return TRUE; }
	virtual void* Create(BOOL /*loading = FALSE*/) 		{ return new maxPlugin(); }
	virtual const TCHAR *	ClassName() 			{ return GetString(IDS_CLASS_NAME); }
	virtual SClass_ID SuperClassID() 				{ return OSM_CLASS_ID; }
	virtual Class_ID ClassID() 						{ return maxPlugin_CLASS_ID; }
	virtual const TCHAR* Category() 				{ return GetString(IDS_CATEGORY); }

	virtual const TCHAR* InternalName() 			{ return _T("maxPlugin"); }	// returns fixed parsable name (scripter-visible name)
	virtual HINSTANCE HInstance() 					{ return hInstance; }					// returns owning module handle
	

};


ClassDesc2* GetmaxPluginDesc() { 
	static maxPluginClassDesc maxPluginDesc;
	return &maxPluginDesc; 
}




enum { maxplugin_params };


enum { 
	pb_spin,
	p_caster,
	p_receiver,
	p_intList,
	p_floatList,
	p_strList,
	p_vec3List
};


static ParamBlockDesc2 maxplugin_param_blk ( maxplugin_params, _T("params"),  0, GetmaxPluginDesc(), 
	P_AUTO_CONSTRUCT + P_AUTO_UI, PBLOCK_REF, 
	//rollout
	IDD_PANEL, IDS_PARAMS, 0, 0, NULL,
	 //params

	p_caster, _T("caster"), TYPE_BOOL, P_RESET_DEFAULT,  IDS_CASTER,
		p_default, TRUE,
		p_ui, TYPE_SINGLECHECKBOX, IDC_CHECK1,
	p_end,

	p_receiver, _T("receiver"), TYPE_BOOL, P_RESET_DEFAULT, IDS_RECEIVER,
		p_default, TRUE,
		p_ui, TYPE_SINGLECHECKBOX, IDC_CHECK2,
	p_end,


	p_intList, _T("intList"), TYPE_INT_TAB, 0, P_ANIMATABLE | P_VARIABLE_SIZE, IDS_INTLIST,
	p_range, -99999999, 99999999,
	p_ui, TYPE_INTLISTBOX, IDC_INT_LIST, IDC_CUST_INT_ADD2, IDC_CUST_INT_REPLACE2, IDC_CUST_INT_DELETE2, EDITTYPE_INT, IDC_CUST_INT_NEWVAL2, IDCS_CUST_INT_NEWVAL2, SPIN_AUTOSCALE,
	p_end,

	p_floatList, _T("floatList"), TYPE_FLOAT_TAB, 0, P_ANIMATABLE | P_VARIABLE_SIZE, IDS_FLOATLIST,
	p_range, -99999999.0, 99999999.0,
	p_ui, TYPE_FLOATLISTBOX, IDC_FLOAT_LIST, IDC_CUST_FLOAT_ADD, IDC_CUST_FLOAT_REPLACE, IDC_CUST_FLOAT_DELETE, EDITTYPE_FLOAT, IDC_CUST_FLOAT_NEWVAL, IDCS_CUST_FLOAT_NEWVAL, SPIN_AUTOSCALE,
	p_end,

	p_strList, _T("strList"), TYPE_STRING_TAB, 0, P_ANIMATABLE | P_VARIABLE_SIZE, IDS_STRINGLIST,
	p_ui, TYPE_STRINGLISTBOX, IDC_STR_LIST, IDC_CUST_STR_ADD, IDC_CUST_STR_REPLACE,
	IDC_CUST_STR_DELETE, IDC_CUST_STR_NEWVAL,
	p_end,


	p_vec3List, _T("vec3List"), TYPE_POINT3_TAB, 0, P_ANIMATABLE | P_VARIABLE_SIZE, IDS_VEC3LIST,
	p_range, -99999999.0, 99999999.0,
	p_ui, TYPE_POINT3LISTBOX, IDC_VEC3LIST, IDC_CUST_VEC3_ADD, IDC_CUST_VEC3_REPLACE, IDC_CUST_VEC3_DELETE, EDITTYPE_FLOAT, IDC_CUST_VEC3_X, IDCS_CUST_VEC3_X,
	 IDC_CUST_VEC3_Y, IDCS_CUST_VEC3_Y,
	 IDC_CUST_VEC3_Z, IDCS_CUST_VEC3_Z,
	SPIN_AUTOSCALE,
	p_end,

	p_end
	);




//--- maxPlugin -------------------------------------------------------
maxPlugin::maxPlugin()
	: pblock(nullptr)
{
	GetmaxPluginDesc()->MakeAutoParamBlocks(this);
}

maxPlugin::~maxPlugin()
{
}

/*===========================================================================*\
 |	The validity of the parameters.  First a test for editing is performed
 |  then Start at FOREVER, and intersect with the validity of each item
\*===========================================================================*/
Interval maxPlugin::LocalValidity(TimeValue /*t*/)
{
	// if being edited, return NEVER forces a cache to be built 
	// after previous modifier.
	if (TestAFlag(A_MOD_BEING_EDITED))
		return NEVER;  
	else
	{
		Interval ivalid = FOREVER;

		//GetParamBlock()->GetValue
		//BOOL b;
		//pblock->GetValue(p_caster, t, b, ivalid);

		return ivalid;
	}
	//#pragma message(TODO("Return the validity interval of the modifier"))

}


/*************************************************************************************************
*
	Between NotifyPreCollapse and NotifyPostCollapse, Modify is
	called by the system.  NotifyPreCollapse can be used to save any plugin dependant data e.g.
	LocalModData
*
\*************************************************************************************************/

void maxPlugin::NotifyPreCollapse(INode* /*node*/, IDerivedObject* /*derObj*/, int /*index*/)
{
	//#pragma message(TODO("Perform any Pre Stack Collapse methods here"))
}



/*************************************************************************************************
*
	NotifyPostCollapse can be used to apply the modifier back onto to the stack, copying over the
	stored data from the temporary storage.  To reapply the modifier the following code can be 
	used

	Object *bo = node->GetObjectRef();
	IDerivedObject *derob = NULL;
	if(bo->SuperClassID() != GEN_DERIVOB_CLASS_ID)
	{
		derob = CreateDerivedObject(obj);
		node->SetObjectRef(derob);
	}
	else
		derob = (IDerivedObject*) bo;

	// Add ourselves to the top of the stack
	derob->AddModifier(this,NULL,derob->NumModifiers());

*
\*************************************************************************************************/

void maxPlugin::NotifyPostCollapse(INode* /*node*/,Object* /*obj*/, IDerivedObject* /*derObj*/, int /*index*/)
{
	#pragma message(TODO("Perform any Post Stack collapse methods here."))

}


/*************************************************************************************************
*
	ModifyObject will do all the work in a full modifier
    This includes casting objects to their correct form, doing modifications
	changing their parameters, etc
*
\************************************************************************************************/


void maxPlugin::ModifyObject(TimeValue /*t*/, ModContext& /*mc*/, ObjectState* /*os*/, INode* /*node*/) 
{
	//#pragma message(TODO("Add the code for actually modifying the object"))
}


void maxPlugin::BeginEditParams( IObjParam* ip, ULONG flags, Animatable* prev )
{
	TimeValue t = ip->GetTime();
	NotifyDependents(Interval(t,t), PART_ALL, REFMSG_BEGIN_EDIT);
	NotifyDependents(Interval(t,t), PART_ALL, REFMSG_MOD_DISPLAY_ON);
	SetAFlag(A_MOD_BEING_EDITED);	

	//ReplaceReference()

	GetmaxPluginDesc()->BeginEditParams(ip, this, flags, prev);
}

void maxPlugin::EndEditParams( IObjParam *ip, ULONG flags, Animatable *next)
{
	GetmaxPluginDesc()->EndEditParams(ip, this, flags, next);

	TimeValue t = ip->GetTime();

	ClearAFlag(A_MOD_BEING_EDITED);
	NotifyDependents(Interval(t,t), PART_ALL, REFMSG_END_EDIT);
	NotifyDependents(Interval(t,t), PART_ALL, REFMSG_MOD_DISPLAY_OFF);
}



Interval maxPlugin::GetValidity(TimeValue t)
{
	Interval valid = FOREVER;

	float f;
	// Intersect each parameters interval to narrow it down.
	//GetParamBlock(0)->GetValue(pb_spin, t, f, valid);
	GetParamBlock(0)->GetValue(p_caster, t, f, valid);
	GetParamBlock(0)->GetValue(p_receiver, t, f, valid);

	return valid;
}




RefTargetHandle maxPlugin::Clone(RemapDir& remap)
{
	maxPlugin* newmod = new maxPlugin();
	#pragma message(TODO("Add the cloning code here"))
	newmod->ReplaceReference(PBLOCK_REF,remap.CloneRef(pblock));
	BaseClone(this, newmod, remap);
	return(newmod);
}


//From ReferenceMaker 
RefResult maxPlugin::NotifyRefChanged(
		const Interval& /*changeInt*/, RefTargetHandle hTarget,
		PartID& /*partID*/,  RefMessage message, BOOL /*propagate*/) 
{
	#pragma message(TODO("Add code to handle the various reference changed messages"))
	switch (message)
	{
	case REFMSG_TARGET_DELETED:
		{
			if (hTarget == pblock)
			{
				pblock = nullptr;
			}
		}
		break;
	}
	return REF_SUCCEED;
}

/****************************************************************************************
*
 	NotifyInputChanged is called each time the input object is changed in some way
 	We can find out how it was changed by checking partID and message
*
\****************************************************************************************/

void maxPlugin::NotifyInputChanged(const Interval &/*changeInt*/, PartID /*partID*/, RefMessage /*message*/, ModContext* /*mc*/)
{

}



//From Object
BOOL maxPlugin::HasUVW() 
{ 
	#pragma message(TODO("Return whether the object has UVW coordinates or not"))
	return TRUE; 
}

void maxPlugin::SetGenUVW(BOOL sw) 
{  
	if (sw==HasUVW()) 
		return;
	#pragma message(TODO("Set the plugin internal value to sw"))
}

IOResult maxPlugin::Load(ILoad* /*iload*/)
{
	#pragma message(TODO("Add code to allow plugin to load its data"))
	
	return IO_OK;
}

IOResult maxPlugin::Save(ISave* isave)
{
	#pragma message(TODO("Add code to allow plugin to save its data"))
	
	return IO_OK;
}

