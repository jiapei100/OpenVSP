//
// This file is released under the terms of the NASA Open Source Agreement (NOSA)
// version 1.3 as detailed in the LICENSE file which accompanies this software.
//

// FeaPartEditScreen.cpp FeaPart editor screen.
// Justin Gravett
//
//////////////////////////////////////////////////////////////////////

#include "FeaPartEditScreen.h"
#include "StructureMgr.h"
#include "SubSurface.h"
#include "FeaMeshMgr.h"

FeaPartEditScreen::FeaPartEditScreen( ScreenMgr* mgr ) : BasicScreen( mgr, 310, 365, "FEA Part Edit" )
{
    m_FLTK_Window->callback( staticCloseCB, this );

    //=== FEA Parts ===//
    m_GenLayout.SetGroupAndScreen( m_FLTK_Window, this );

    m_GenLayout.AddY( m_GenLayout.GetStdHeight() + m_GenLayout.GetGapHeight() );

    m_GenLayout.AddInput( m_FeaPartNameInput, "Part Name" );

    m_GenLayout.AddYGap();

    m_CurFeaPartDispGroup = NULL;

    // Indivdual FeaPart Parameters:
    int start_y = m_GenLayout.GetY();

    //==== FeaSlice ====//
    m_GenLayout.AddSubGroupLayout( m_SkinEditLayout, m_GenLayout.GetW(), m_GenLayout.GetH() );
    m_SkinEditLayout.SetY( start_y );

    m_SkinEditLayout.AddDividerBox( "Skin" );

    m_SkinEditLayout.AddButton( m_RemoveSkinTrisToggle, "Remove Skin Tris" );

    m_SkinEditLayout.AddYGap();

    m_SkinEditLayout.AddChoice( m_SkinPropertyChoice, "Property" );

    //==== FeaSlice ====//
    m_GenLayout.AddSubGroupLayout( m_SliceEditLayout, m_GenLayout.GetW(), m_GenLayout.GetH() );
    m_SliceEditLayout.SetY( start_y );

    m_SliceEditLayout.AddDividerBox( "Slice" );

    m_SliceEditLayout.SetButtonWidth( m_SliceEditLayout.GetRemainX() / 3 );
    m_SliceEditLayout.SetChoiceButtonWidth( m_SliceEditLayout.GetRemainX() / 3 );

    m_SliceOrientationChoice.AddItem( "XY Plane" );
    m_SliceOrientationChoice.AddItem( "YZ Plane" );
    m_SliceOrientationChoice.AddItem( "XZ Plane" );
    m_SliceEditLayout.AddChoice( m_SliceOrientationChoice, "Orientation" );

    m_SliceEditLayout.AddSlider( m_SliceCenterLocSlider, "Position", 1, "%5.3f" );
    m_SliceEditLayout.AddSlider( m_SliceThetaSlider, "Theta", 25, "%5.3f" );
    m_SliceEditLayout.AddSlider( m_SliceAlphaSlider, "Alpha", 25, "%5.3f" );

    m_SliceEditLayout.AddYGap();

    m_SliceEditLayout.AddChoice( m_SlicePropertyChoice, "Property" );
    m_SliceEditLayout.AddChoice( m_SliceCapPropertyChoice, "Cap Property" );

    //==== FeaRib ====//
    m_GenLayout.AddSubGroupLayout( m_RibEditLayout, m_GenLayout.GetW(), m_GenLayout.GetH() );
    m_RibEditLayout.SetY( start_y );

    m_RibEditLayout.AddDividerBox( "Rib" );

    m_RibEditLayout.SetButtonWidth( m_RibEditLayout.GetRemainX() / 3 );
    m_RibEditLayout.SetChoiceButtonWidth( m_RibEditLayout.GetRemainX() / 3 );

    m_RibEditLayout.AddChoice( m_RibPerpendicularEdgeChoice, "Perpendicular Edge" );

    m_RibEditLayout.AddSlider( m_RibPosSlider, "Position", 1, "%5.3f" );
    m_RibEditLayout.AddSlider( m_RibThetaSlider, "Theta", 25, "%5.3f" );

    m_RibEditLayout.AddYGap();

    m_RibEditLayout.AddChoice( m_RibPropertyChoice, "Property" );
    m_RibEditLayout.AddChoice( m_RibCapPropertyChoice, "Cap Property" );

    //==== FeaSpar ====//
    m_GenLayout.AddSubGroupLayout( m_SparEditLayout, m_GenLayout.GetW(), m_GenLayout.GetH() );
    m_SparEditLayout.SetY( start_y );

    m_SparEditLayout.AddDividerBox( "Spar" );

    m_SparEditLayout.SetButtonWidth( m_SparEditLayout.GetRemainX() / 3 );
    m_SparEditLayout.SetChoiceButtonWidth( m_SparEditLayout.GetRemainX() / 3 );

    m_SparEditLayout.AddSlider( m_SparPosSlider, "Position", 1, "%5.3f" );
    m_SparEditLayout.AddSlider( m_SparThetaSlider, "Theta", 25, "%5.3f" );

    m_SparEditLayout.AddYGap();

    m_SparEditLayout.AddChoice( m_SparPropertyChoice, "Property" );
    m_SparEditLayout.AddChoice( m_SparCapPropertyChoice, "Cap Property" );

    //==== FeaFixPoint ====//
    m_GenLayout.AddSubGroupLayout( m_FixPointEditLayout, m_GenLayout.GetW(), m_GenLayout.GetH() );
    m_FixPointEditLayout.SetY( start_y );

    m_FixPointEditLayout.AddDividerBox( "Fixed Point" );

    m_FixPointEditLayout.SetChoiceButtonWidth( m_FixPointEditLayout.GetRemainX() / 3 );
    m_FixPointEditLayout.SetButtonWidth( m_FixPointEditLayout.GetRemainX() / 3 );

    m_FixPointEditLayout.AddChoice( m_FixPointParentSurfChoice, "Parent Surface" );

    m_FixPointEditLayout.AddSlider( m_FixPointULocSlider, "U Location", 1, "%5.3f" );
    m_FixPointEditLayout.AddSlider( m_FixPointWLocSlider, "W Location", 1, "%5.3f" );

    m_FixPointEditLayout.AddYGap();

    m_FixPointEditLayout.SetSameLineFlag( true );
    m_FixPointEditLayout.SetFitWidthFlag( false );

    m_FixPointEditLayout.SetButtonWidth( m_FixPointEditLayout.GetRemainX() / 17 );

    m_FixPointEditLayout.AddButton( m_FixPointMassToggle, "" );

    m_FixPointEditLayout.SetButtonWidth( ( m_FixPointEditLayout.GetW() / 3 ) - ( m_FixPointEditLayout.GetRemainX() / 16 ) );
    m_FixPointEditLayout.SetSliderWidth( m_FixPointEditLayout.GetW() / 3 );
    m_FixPointEditLayout.SetInputWidth(  m_FixPointEditLayout.GetRemainX() / 6 );

    m_FixPointEditLayout.AddSlider( m_FixPointMassSlider, "Mass", 100.0, "%5.3g" );

    m_FixPointEditLayout.AddButton( m_FixPointMassUnit, "" );
    m_FixPointMassUnit.GetFlButton()->box( FL_THIN_UP_BOX );
    m_FixPointMassUnit.GetFlButton()->labelcolor( FL_BLACK );
    m_FixPointMassUnit.SetWidth( ( m_FixPointEditLayout.GetW() / 8 ) - 3 );

    //=== SubSurfaces ===//

    //==== SSLine ====//
    m_GenLayout.AddSubGroupLayout( m_FeaSSLineGroup, m_GenLayout.GetW(), m_GenLayout.GetH() );
    m_FeaSSLineGroup.SetY( start_y );

    m_FeaSSLineGroup.AddDividerBox( "Line Sub-Surface" );

    m_FeaSSLineGroup.SetFitWidthFlag( false );
    m_FeaSSLineGroup.SetSameLineFlag( true );
    m_FeaSSLineGroup.AddLabel( "Tag", m_FeaSSLineGroup.GetRemainX() / 3 );
    m_FeaSSLineGroup.SetButtonWidth( m_FeaSSLineGroup.GetRemainX() / 3 );
    m_FeaSSLineGroup.AddButton( m_FeaSSLineGreaterToggle, "Greater" );
    m_FeaSSLineGroup.AddButton( m_FeaSSLineLessToggle, "Less" );
    m_FeaSSLineGroup.AddButton( m_FeaSSLineNoneToggle, "None" );

    m_FeaSSLineTestToggleGroup.Init( this );
    m_FeaSSLineTestToggleGroup.AddButton( m_FeaSSLineGreaterToggle.GetFlButton() );
    m_FeaSSLineTestToggleGroup.AddButton( m_FeaSSLineLessToggle.GetFlButton() );
    m_FeaSSLineTestToggleGroup.AddButton( m_FeaSSLineNoneToggle.GetFlButton() );

    m_FeaSSLineGroup.ForceNewLine();

    m_FeaSSLineGroup.AddLabel( "Line Type", m_FeaSSLineGroup.GetRemainX() / 3 );
    m_FeaSSLineGroup.SetButtonWidth( m_FeaSSLineGroup.GetRemainX() / 2 );
    m_FeaSSLineGroup.AddButton( m_FeaSSLineConstUButton, "U" );
    m_FeaSSLineGroup.AddButton( m_FeaSSLineConstWButton, "W" );

    m_FeaSSLineConstToggleGroup.Init( this );
    m_FeaSSLineConstToggleGroup.AddButton( m_FeaSSLineConstUButton.GetFlButton() );
    m_FeaSSLineConstToggleGroup.AddButton( m_FeaSSLineConstWButton.GetFlButton() );

    m_FeaSSLineGroup.ForceNewLine();
    m_FeaSSLineGroup.SetFitWidthFlag( true );
    m_FeaSSLineGroup.SetSameLineFlag( false );

    m_FeaSSLineGroup.SetButtonWidth( m_FeaSSLineGroup.GetRemainX() / 3 );
    m_FeaSSLineGroup.SetChoiceButtonWidth( m_FeaSSLineGroup.GetRemainX() / 3 );

    m_FeaSSLineGroup.SetButtonWidth( m_FeaSSLineGroup.GetRemainX() / 3 );
    m_FeaSSLineGroup.AddSlider( m_FeaSSLineConstSlider, "Value", 1, "%5.4f" );

    m_FeaSSLineGroup.AddYGap();

    m_FeaSSLineGroup.AddChoice( m_FeaSSLinePropertyChoice, "Property" );
    m_FeaSSLineGroup.AddChoice( m_FeaSSLineCapPropertyChoice, "Cap Property" );

    //==== SSRectangle ====//
    m_GenLayout.AddSubGroupLayout( m_FeaSSRecGroup, m_GenLayout.GetW(), m_GenLayout.GetH() );
    m_FeaSSRecGroup.SetY( start_y );

    m_FeaSSRecGroup.AddDividerBox( "Rectangle Sub-Surface" );

    m_FeaSSRecGroup.SetFitWidthFlag( false );
    m_FeaSSRecGroup.SetSameLineFlag( true );
    m_FeaSSRecGroup.AddLabel( "Tag", m_FeaSSRecGroup.GetRemainX() / 3 );
    m_FeaSSRecGroup.SetButtonWidth( m_FeaSSRecGroup.GetRemainX() / 3 );
    m_FeaSSRecGroup.AddButton( m_FeaSSRecInsideButton, "Inside" );
    m_FeaSSRecGroup.AddButton( m_FeaSSRecOutsideButton, "Outside" );
    m_FeaSSRecGroup.AddButton( m_FeaSSRecNoneButton, "None" );

    m_FeaSSRecTestToggleGroup.Init( this );
    m_FeaSSRecTestToggleGroup.AddButton( m_FeaSSRecInsideButton.GetFlButton() );
    m_FeaSSRecTestToggleGroup.AddButton( m_FeaSSRecOutsideButton.GetFlButton() );
    m_FeaSSRecTestToggleGroup.AddButton( m_FeaSSRecNoneButton.GetFlButton() );

    m_FeaSSRecGroup.ForceNewLine();
    m_FeaSSRecGroup.SetFitWidthFlag( true );
    m_FeaSSRecGroup.SetSameLineFlag( false );

    m_FeaSSRecGroup.SetButtonWidth( m_FeaSSRecGroup.GetRemainX() / 3 );
    m_FeaSSRecGroup.SetChoiceButtonWidth( m_FeaSSRecGroup.GetRemainX() / 3 );

    m_FeaSSRecGroup.AddSlider( m_FeaSSRecCentUSlider, "Center U", 1, "%5.4f" );
    m_FeaSSRecGroup.AddSlider( m_FeaSSRecCentWSlider, "Center W", 1, "%5.4f" );
    m_FeaSSRecGroup.AddSlider( m_FeaSSRecULenSlider, "U Length", 1, "%5.4f" );
    m_FeaSSRecGroup.AddSlider( m_FeaSSRecWLenSlider, "W Length", 1, "%5.4f" );
    m_FeaSSRecGroup.AddSlider( m_FeaSSRecThetaSlider, "Theta", 25, "%5.4f" );

    m_FeaSSRecGroup.AddYGap();

    m_FeaSSRecGroup.AddChoice( m_FeaSSRecPropertyChoice, "Property" );
    m_FeaSSRecGroup.AddChoice( m_FeaSSRecCapPropertyChoice, "Cap Property" );

    //==== SS_Ellipse ====//
    m_GenLayout.AddSubGroupLayout( m_FeaSSEllGroup, m_GenLayout.GetW(), m_GenLayout.GetH() );
    m_FeaSSEllGroup.SetY( start_y );

    m_FeaSSEllGroup.AddDividerBox( "Ellipse Sub-Surface" );

    m_FeaSSEllGroup.SetFitWidthFlag( false );
    m_FeaSSEllGroup.SetSameLineFlag( true );
    m_FeaSSEllGroup.AddLabel( "Tag", m_FeaSSEllGroup.GetRemainX() / 3 );
    m_FeaSSEllGroup.SetButtonWidth( m_FeaSSEllGroup.GetRemainX() / 3 );
    m_FeaSSEllGroup.AddButton( m_FeaSSEllInsideButton, "Inside" );
    m_FeaSSEllGroup.AddButton( m_FeaSSEllOutsideButton, "Outside" );
    m_FeaSSEllGroup.AddButton( m_FeaSSEllNoneButton, "None" );

    m_FeaSSEllTestToggleGroup.Init( this );
    m_FeaSSEllTestToggleGroup.AddButton( m_FeaSSEllInsideButton.GetFlButton() );
    m_FeaSSEllTestToggleGroup.AddButton( m_FeaSSEllOutsideButton.GetFlButton() );
    m_FeaSSEllTestToggleGroup.AddButton( m_FeaSSEllNoneButton.GetFlButton() );

    m_FeaSSEllGroup.ForceNewLine();
    m_FeaSSEllGroup.SetFitWidthFlag( true );
    m_FeaSSEllGroup.SetSameLineFlag( false );

    m_FeaSSEllGroup.SetButtonWidth( m_FeaSSEllGroup.GetRemainX() / 3 );
    m_FeaSSEllGroup.SetChoiceButtonWidth( m_FeaSSEllGroup.GetRemainX() / 3 );

    m_FeaSSEllGroup.AddSlider( m_FeaSSEllTessSlider, "Num Points", 100, "%5.0f" );
    m_FeaSSEllGroup.AddSlider( m_FeaSSEllCentUSlider, "Center U", 1, "%5.4f" );
    m_FeaSSEllGroup.AddSlider( m_FeaSSEllCentWSlider, "Center W", 1, "%5.4f" );
    m_FeaSSEllGroup.AddSlider( m_FeaSSEllULenSlider, "U Length", 1, "%5.4f" );
    m_FeaSSEllGroup.AddSlider( m_FeaSSEllWLenSlider, "W Length", 1, "%5.4f" );
    m_FeaSSEllGroup.AddSlider( m_FeaSSEllThetaSlider, "Theta", 25, "%5.4f" );

    m_FeaSSEllGroup.AddYGap();

    m_FeaSSEllGroup.AddChoice( m_FeaSSEllPropertyChoice, "Property" );
    m_FeaSSEllGroup.AddChoice( m_FeaSSEllCapPropertyChoice, "Cap Property" );

    //===== SSControl ====//
    m_GenLayout.AddSubGroupLayout( m_FeaSSConGroup, m_GenLayout.GetW(), m_GenLayout.GetH() );
    m_FeaSSConGroup.SetY( start_y );

    m_FeaSSConGroup.AddDividerBox( "Control Sub-Surface" );

    m_FeaSSConGroup.SetFitWidthFlag( false );
    m_FeaSSConGroup.SetSameLineFlag( true );
    m_FeaSSConGroup.AddLabel( "Tag", m_FeaSSConGroup.GetRemainX() / 3 );
    m_FeaSSConGroup.SetButtonWidth( m_FeaSSConGroup.GetRemainX() / 3 );
    m_FeaSSConGroup.AddButton( m_FeaSSConInsideButton, "Inside" );
    m_FeaSSConGroup.AddButton( m_FeaSSConOutsideButton, "Outside" );
    m_FeaSSConGroup.AddButton( m_FeaSSConNoneButton, "None" );

    m_FeaSSConTestToggleGroup.Init( this );
    m_FeaSSConTestToggleGroup.AddButton( m_FeaSSConInsideButton.GetFlButton() );
    m_FeaSSConTestToggleGroup.AddButton( m_FeaSSConOutsideButton.GetFlButton() );
    m_FeaSSConTestToggleGroup.AddButton( m_FeaSSConNoneButton.GetFlButton() );

    m_FeaSSConGroup.ForceNewLine();

    m_FeaSSConGroup.SetChoiceButtonWidth( m_FeaSSConGroup.GetRemainX() / 3 );

    m_FeaSSConSurfTypeChoice.AddItem( "Upper" );
    m_FeaSSConSurfTypeChoice.AddItem( "Lower" );
    m_FeaSSConSurfTypeChoice.AddItem( "Both" );
    m_FeaSSConGroup.AddChoice( m_FeaSSConSurfTypeChoice, "Upper/Lower", m_FeaSSConGroup.GetRemainX() / 3 );

    m_FeaSSConGroup.SetButtonWidth( m_FeaSSConGroup.GetRemainX() );

    m_FeaSSConGroup.AddButton( m_FeaSSConLEFlagButton, "Leading Edge" );

    m_FeaSSConGroup.SetFitWidthFlag( true );
    m_FeaSSConGroup.SetSameLineFlag( false );
    m_FeaSSConGroup.ForceNewLine();

    m_FeaSSConGroup.AddYGap();
    m_FeaSSConGroup.AddDividerBox( "Spanwise" );

    m_FeaSSConGroup.SetButtonWidth( m_FeaSSConGroup.GetRemainX() / 3 );

    m_FeaSSConGroup.AddSlider( m_FeaSSConUSSlider, "Start U", 1, "%5.4f" );
    m_FeaSSConGroup.AddSlider( m_FeaSSConUESlider, "End U", 1, "%5.4f" );

    m_FeaSSConGroup.AddYGap();
    m_FeaSSConGroup.AddDividerBox( "Chordwise" );

    m_FeaSSConGroup.SetFitWidthFlag( false );
    m_FeaSSConGroup.SetSameLineFlag( true );

    m_FeaSSConGroup.SetButtonWidth( m_FeaSSConGroup.GetW() / 3 );

    m_FeaSSConGroup.AddButton( m_FeaSSConSAbsButton, "Length" );
    m_FeaSSConGroup.AddButton( m_FeaSSConSRelButton, "Length/C" );
    m_FeaSSConGroup.AddButton( m_FeaSSConSEConstButton, "Constant" );

    m_FeaSSConGroup.SetFitWidthFlag( true );
    m_FeaSSConGroup.SetSameLineFlag( false );
    m_FeaSSConGroup.ForceNewLine();

    m_FeaSSConSAbsRelToggleGroup.Init( this );
    m_FeaSSConSAbsRelToggleGroup.AddButton( m_FeaSSConSAbsButton.GetFlButton() );
    m_FeaSSConSAbsRelToggleGroup.AddButton( m_FeaSSConSRelButton.GetFlButton() );

    m_FeaSSConGroup.SetButtonWidth( m_FeaSSConGroup.GetRemainX() / 3 );

    m_FeaSSConGroup.AddSlider( m_FeaSSConSLenSlider, "Start Length", 10.0, "%5.4f" );
    m_FeaSSConGroup.AddSlider( m_FeaSSConSFracSlider, "Start Length/C", 1.0, "%5.4f" );
    m_FeaSSConGroup.AddSlider( m_FeaSSConELenSlider, "End Length", 10.0, "%5.4f" );
    m_FeaSSConGroup.AddSlider( m_FeaSSConEFracSlider, "End Length/C", 1.0, "%5.4f" );

    m_FeaSSConGroup.AddYGap();

    m_FeaSSConGroup.AddChoice( m_FeaSSConPropertyChoice, "Property" );
    m_FeaSSConGroup.AddChoice( m_FeaSSConCapPropertyChoice, "Cap Property" );

}

FeaPartEditScreen::~FeaPartEditScreen()
{
}

bool FeaPartEditScreen::Update()
{
    assert( m_ScreenMgr );

    Vehicle* veh = m_ScreenMgr->GetVehiclePtr();

    if ( veh )
    {
        //==== Update Perpendicular Rib Choice ====//
        UpdatePerpendicularRibChoice();

        //==== Update FixPoint Parent Surf Choice ====//
        UpdateFixPointParentChoice();

        //===== FeaProperty Update =====//
        UpdateFeaPropertyChoice();

        UpdateUnitLabels();

        if ( StructureMgr.ValidTotalFeaStructInd( StructureMgr.GetCurrStructIndex() ) )
        {
            vector< FeaStructure* > structVec = StructureMgr.GetAllFeaStructs();

            // Update Current FeaPart
            if ( StructureMgr.GetCurrPartIndex() < structVec[StructureMgr.GetCurrStructIndex()]->NumFeaParts() )
            {
                FeaPart* feaprt = structVec[StructureMgr.GetCurrStructIndex()]->GetFeaPart( StructureMgr.GetCurrPartIndex() );
                if ( feaprt )
                {
                    m_FeaPartNameInput.Update( feaprt->GetName() );

                    if ( feaprt->GetType() == vsp::FEA_SKIN )
                    {
                        FeaSkin* skin = dynamic_cast<FeaSkin*>( feaprt );
                        assert( skin );

                        m_RemoveSkinTrisToggle.Update( skin->m_RemoveSkinTrisFlag.GetID() );

                        FeaPartDispGroup( &m_SkinEditLayout );
                    }
                    else if ( feaprt->GetType() == vsp::FEA_SLICE )
                    {
                        FeaSlice* slice = dynamic_cast<FeaSlice*>( feaprt );
                        assert( slice );

                        m_SliceOrientationChoice.Update( slice->m_OrientationPlane.GetID() );
                        m_SliceCenterLocSlider.Update( slice->m_CenterPerBBoxLocation.GetID() );
                        m_SliceThetaSlider.Update( slice->m_Theta.GetID() );
                        m_SliceAlphaSlider.Update( slice->m_Alpha.GetID() );

                        if ( slice->m_IncludedElements() == TRIS || slice->m_IncludedElements() == BOTH_ELEMENTS )
                        {
                            m_SlicePropertyChoice.Activate();
                        }
                        else
                        {
                            m_SlicePropertyChoice.Deactivate();
                        }

                        if ( slice->m_IncludedElements() == BEAM || slice->m_IncludedElements() == BOTH_ELEMENTS )
                        {
                            m_SliceCapPropertyChoice.Activate();
                        }
                        else
                        {
                            m_SliceCapPropertyChoice.Deactivate();
                        }

                        FeaPartDispGroup( &m_SliceEditLayout );
                    }
                    else if ( feaprt->GetType() == vsp::FEA_RIB )
                    {
                        FeaRib* rib = dynamic_cast<FeaRib*>( feaprt );
                        assert( rib );

                        m_RibPosSlider.Update( rib->m_PerU.GetID() );
                        m_RibThetaSlider.Update( rib->m_Theta.GetID() );

                        if ( rib->m_IncludedElements() == BEAM || rib->m_IncludedElements() == BOTH_ELEMENTS )
                        {
                            m_RibCapPropertyChoice.Activate();
                        }
                        else
                        {
                            m_RibCapPropertyChoice.Deactivate();
                        }

                        FeaPartDispGroup( &m_RibEditLayout );
                    }
                    else if ( feaprt->GetType() == vsp::FEA_SPAR )
                    {
                        FeaSpar* spar = dynamic_cast<FeaSpar*>( feaprt );
                        assert( spar );

                        m_SparPosSlider.Update( spar->m_PerV.GetID() );
                        m_SparThetaSlider.Update( spar->m_Theta.GetID() );

                        if ( spar->m_IncludedElements() == BEAM || spar->m_IncludedElements() == BOTH_ELEMENTS )
                        {
                            m_SparCapPropertyChoice.Activate();
                        }
                        else
                        {
                            m_SparCapPropertyChoice.Deactivate();
                        }

                        FeaPartDispGroup( &m_SparEditLayout );
                    }
                    else if ( feaprt->GetType() == vsp::FEA_FIX_POINT )
                    {
                        FeaFixPoint* fixpt = dynamic_cast<FeaFixPoint*>( feaprt );
                        assert( fixpt );

                        m_FixPointULocSlider.Update( fixpt->m_PosU.GetID() );
                        m_FixPointWLocSlider.Update( fixpt->m_PosW.GetID() );
                        m_FixPointMassToggle.Update( fixpt->m_FixPointMassFlag.GetID() );
                        m_FixPointMassSlider.Update( fixpt->m_FixPointMass.GetID() );

                        if ( fixpt->m_FixPointMassFlag() )
                        {
                            m_FixPointMassSlider.Activate();
                            m_FixPointMassUnit.Activate();
                        }
                        else
                        {
                            m_FixPointMassSlider.Deactivate();
                            m_FixPointMassUnit.Deactivate();
                        }

                        FeaPartDispGroup( &m_FixPointEditLayout );
                    }
                    else
                    {
                        FeaPartDispGroup( NULL );
                    }
                }
                else
                {
                    FeaPartDispGroup( NULL );
                }
            }
            else if ( StructureMgr.GetCurrPartIndex() >= structVec[StructureMgr.GetCurrStructIndex()]->NumFeaParts() )
            {
                //===== SubSurface =====//
                SubSurface* subsurf = structVec[StructureMgr.GetCurrStructIndex()]->GetFeaSubSurf( StructureMgr.GetCurrPartIndex() - structVec[StructureMgr.GetCurrStructIndex()]->NumFeaParts() );
                if ( subsurf )
                {
                    m_FeaPartNameInput.Update( subsurf->GetName() );
                    m_ShellCapToggleGroup.Update( subsurf->m_IncludedElements.GetID() );

                    if ( subsurf->GetType() == vsp::SS_LINE )
                    {
                        SSLine* ssline = dynamic_cast<SSLine*>( subsurf );
                        assert( ssline );

                        m_FeaSSLineConstToggleGroup.Update( ssline->m_ConstType.GetID() );
                        m_FeaSSLineTestToggleGroup.Update( ssline->m_TestType.GetID() );
                        m_FeaSSLineConstSlider.Update( ssline->m_ConstVal.GetID() );

                        m_FeaSSLinePropertyChoice.Activate();

                        if ( ssline->m_TestType() == vsp::NONE )
                        {
                            m_FeaSSLinePropertyChoice.Deactivate();
                        }

                        if ( ssline->m_IncludedElements() == TRIS || ssline->m_IncludedElements() == BOTH_ELEMENTS )
                        {
                            m_FeaSSLinePropertyChoice.Activate();
                        }
                        else
                        {
                            m_FeaSSLinePropertyChoice.Deactivate();
                        }

                        if ( ssline->m_IncludedElements() == BEAM || ssline->m_IncludedElements() == BOTH_ELEMENTS )
                        {
                            m_FeaSSLineCapPropertyChoice.Activate();
                        }
                        else
                        {
                            m_FeaSSLineCapPropertyChoice.Deactivate();
                        }

                        FeaPartDispGroup( &m_FeaSSLineGroup );
                    }
                    else if ( subsurf->GetType() == vsp::SS_RECTANGLE )
                    {
                        SSRectangle* ssrec = dynamic_cast<SSRectangle*>( subsurf );
                        assert( ssrec );

                        m_FeaSSRecTestToggleGroup.Update( ssrec->m_TestType.GetID() );
                        m_FeaSSRecCentUSlider.Update( ssrec->m_CenterU.GetID() );
                        m_FeaSSRecCentWSlider.Update( ssrec->m_CenterW.GetID() );
                        m_FeaSSRecULenSlider.Update( ssrec->m_ULength.GetID() );
                        m_FeaSSRecWLenSlider.Update( ssrec->m_WLength.GetID() );
                        m_FeaSSRecThetaSlider.Update( ssrec->m_Theta.GetID() );

                        if ( ssrec->m_IncludedElements() == BEAM )
                        {
                            m_FeaSSRecCapPropertyChoice.Activate();
                            m_FeaSSRecPropertyChoice.Deactivate();
                            m_FeaSSRecTestToggleGroup.Deactivate();
                        }
                        else if ( ssrec->m_IncludedElements() == TRIS  )
                        {
                            m_FeaSSRecCapPropertyChoice.Deactivate();
                            m_FeaSSRecPropertyChoice.Activate();
                            m_FeaSSRecTestToggleGroup.Activate();
                        }
                        else if ( ssrec->m_IncludedElements() == BOTH_ELEMENTS )
                        {
                            m_FeaSSRecCapPropertyChoice.Activate();
                            m_FeaSSRecPropertyChoice.Activate();
                            m_FeaSSRecTestToggleGroup.Activate();
                        }

                        if ( ssrec->m_TestType() == vsp::NONE )
                        {
                            m_FeaSSRecPropertyChoice.Deactivate();
                        }

                        FeaPartDispGroup( &m_FeaSSRecGroup );
                    }
                    else if ( subsurf->GetType() == vsp::SS_ELLIPSE )
                    {
                        SSEllipse* ssell = dynamic_cast<SSEllipse*>( subsurf );
                        assert( ssell );

                        m_FeaSSEllTestToggleGroup.Update( ssell->m_TestType.GetID() );
                        m_FeaSSEllTessSlider.Update( ssell->m_Tess.GetID() );
                        m_FeaSSEllCentUSlider.Update( ssell->m_CenterU.GetID() );
                        m_FeaSSEllCentWSlider.Update( ssell->m_CenterW.GetID() );
                        m_FeaSSEllULenSlider.Update( ssell->m_ULength.GetID() );
                        m_FeaSSEllWLenSlider.Update( ssell->m_WLength.GetID() );
                        m_FeaSSEllThetaSlider.Update( ssell->m_Theta.GetID() );

                        if ( ssell->m_IncludedElements() == BEAM )
                        {
                            m_FeaSSEllCapPropertyChoice.Activate();
                            m_FeaSSEllPropertyChoice.Deactivate();
                            m_FeaSSEllTestToggleGroup.Deactivate();
                        }
                        else if ( ssell->m_IncludedElements() == TRIS )
                        {
                            m_FeaSSEllCapPropertyChoice.Deactivate();
                            m_FeaSSEllPropertyChoice.Activate();
                            m_FeaSSEllTestToggleGroup.Activate();
                        }
                        else if ( ssell->m_IncludedElements() == BOTH_ELEMENTS )
                        {
                            m_FeaSSEllCapPropertyChoice.Activate();
                            m_FeaSSEllPropertyChoice.Activate();
                            m_FeaSSEllTestToggleGroup.Activate();
                        }

                        if ( ssell->m_TestType() == vsp::NONE )
                        {
                            m_FeaSSEllPropertyChoice.Deactivate();
                        }

                        FeaPartDispGroup( &m_FeaSSEllGroup );
                    }
                    else if ( subsurf->GetType() == vsp::SS_CONTROL )
                    {
                        SSControlSurf* sscon = dynamic_cast<SSControlSurf*>( subsurf );
                        assert( sscon );

                        m_FeaSSConTestToggleGroup.Update( sscon->m_TestType.GetID() );
                        m_FeaSSConUESlider.Update( sscon->m_UEnd.GetID() );
                        m_FeaSSConUSSlider.Update( sscon->m_UStart.GetID() );
                        m_FeaSSConSFracSlider.Update( sscon->m_StartLenFrac.GetID() );
                        m_FeaSSConSLenSlider.Update( sscon->m_StartLength.GetID() );
                        m_FeaSSConEFracSlider.Update( sscon->m_EndLenFrac.GetID() );
                        m_FeaSSConELenSlider.Update( sscon->m_EndLength.GetID() );
                        m_FeaSSConSAbsRelToggleGroup.Update( sscon->m_AbsRelFlag.GetID() );
                        m_FeaSSConSEConstButton.Update( sscon->m_ConstFlag.GetID() );
                        m_FeaSSConLEFlagButton.Update( sscon->m_LEFlag.GetID() );

                        if ( sscon->m_IncludedElements() == BEAM )
                        {
                            m_FeaSSConCapPropertyChoice.Activate();
                            m_FeaSSConPropertyChoice.Deactivate();
                            m_FeaSSConTestToggleGroup.Deactivate();
                        }
                        else if ( sscon->m_IncludedElements() == TRIS )
                        {
                            m_FeaSSConCapPropertyChoice.Deactivate();
                            m_FeaSSConPropertyChoice.Activate();
                            m_FeaSSConTestToggleGroup.Activate();
                        }
                        else if ( sscon->m_IncludedElements() == BOTH_ELEMENTS )
                        {
                            m_FeaSSConCapPropertyChoice.Activate();
                            m_FeaSSConPropertyChoice.Activate();
                            m_FeaSSConTestToggleGroup.Activate();
                        }

                        if ( sscon->m_TestType() == vsp::NONE )
                        {
                            m_FeaSSConPropertyChoice.Deactivate();
                        }

                        m_FeaSSConSFracSlider.Deactivate();
                        m_FeaSSConSLenSlider.Deactivate();
                        m_FeaSSConEFracSlider.Deactivate();
                        m_FeaSSConELenSlider.Deactivate();

                        if ( sscon->m_AbsRelFlag() == vsp::ABS )
                        {
                            m_FeaSSConSLenSlider.Activate();

                            if ( !sscon->m_ConstFlag() )
                            {
                                m_FeaSSConELenSlider.Activate();
                            }
                        }
                        else
                        {
                            m_FeaSSConSFracSlider.Activate();

                            if ( !sscon->m_ConstFlag() )
                            {
                                m_FeaSSConEFracSlider.Activate();
                            }
                        }

                        m_FeaSSConSurfTypeChoice.Update( sscon->m_SurfType.GetID() );
                        FeaPartDispGroup( &m_FeaSSConGroup );
                    }
                }
                else
                {
                    FeaPartDispGroup( NULL );
                }
            }

            // Update FeaParts and SubSurfaces if FeaMesh is not in progress
            if ( !FeaMeshMgr.GetFeaMeshInProgress() )
            {
                structVec[StructureMgr.GetCurrStructIndex()]->Update();
            }
        }
    }

    return true;
}

void FeaPartEditScreen::GuiDeviceCallBack( GuiDevice* device )
{
    assert( m_ScreenMgr );

    Vehicle*  veh = m_ScreenMgr->GetVehiclePtr();

    if ( !veh )
    {
        return;
    }

    if ( device == &m_FeaPartNameInput )
    {
        if ( StructureMgr.ValidTotalFeaStructInd( StructureMgr.GetCurrStructIndex() ) )
        {
            vector < FeaStructure* > structvec = StructureMgr.GetAllFeaStructs();

            if ( StructureMgr.GetCurrPartIndex() < structvec[StructureMgr.GetCurrStructIndex()]->NumFeaParts() )
            {
                FeaPart* feaprt = structvec[StructureMgr.GetCurrStructIndex()]->GetFeaPart( StructureMgr.GetCurrPartIndex() );

                if ( feaprt )
                {
                    feaprt->SetName( m_FeaPartNameInput.GetString() );
                }
            }
            else if ( StructureMgr.GetCurrPartIndex() >= structvec[StructureMgr.GetCurrStructIndex()]->NumFeaParts() )
            {
                SubSurface* subsurf = structvec[StructureMgr.GetCurrStructIndex()]->GetFeaSubSurf( StructureMgr.GetCurrPartIndex() - structvec[StructureMgr.GetCurrStructIndex()]->NumFeaParts() );

                if ( subsurf )
                {
                    subsurf->SetName( m_FeaPartNameInput.GetString() );
                }
            }
        }
    }
    else if ( device == &m_SkinPropertyChoice )
    {
        UpdateFeaPartPropertyIndex( &m_SkinPropertyChoice );
    }
    else if ( device == &m_SlicePropertyChoice )
    {
        UpdateFeaPartPropertyIndex( &m_SlicePropertyChoice );
    }
    else if ( device == &m_SliceCapPropertyChoice )
    {
        UpdateCapPropertyIndex( &m_SliceCapPropertyChoice );
    }
    else if ( device == &m_RibPropertyChoice )
    {
        UpdateFeaPartPropertyIndex( &m_RibPropertyChoice );
    }
    else if ( device == &m_RibCapPropertyChoice )
    {
        UpdateCapPropertyIndex( &m_RibCapPropertyChoice );
    }
    else if ( device == &m_SparPropertyChoice )
    {
        UpdateFeaPartPropertyIndex( &m_SparPropertyChoice );
    }
    else if ( device == &m_SparCapPropertyChoice )
    {
        UpdateCapPropertyIndex( &m_SparCapPropertyChoice );
    }
    else if ( device == &m_FeaSSLinePropertyChoice )
    {
        UpdateFeaPartPropertyIndex( &m_FeaSSLinePropertyChoice );
    }
    else if ( device == &m_FeaSSRecPropertyChoice )
    {
        UpdateFeaPartPropertyIndex( &m_FeaSSRecPropertyChoice );
    }
    else if ( device == &m_FeaSSEllPropertyChoice )
    {
        UpdateFeaPartPropertyIndex( &m_FeaSSEllPropertyChoice );
    }
    else if ( device == &m_FeaSSConPropertyChoice )
    {
        UpdateFeaPartPropertyIndex( &m_FeaSSConPropertyChoice );
    }
    else if ( device == &m_FeaSSLineCapPropertyChoice )
    {
        UpdateCapPropertyIndex( &m_FeaSSLineCapPropertyChoice );
    }
    else if ( device == &m_FeaSSRecCapPropertyChoice )
    {
        UpdateCapPropertyIndex( &m_FeaSSRecCapPropertyChoice );
    }
    else if ( device == &m_FeaSSEllCapPropertyChoice )
    {
        UpdateCapPropertyIndex( &m_FeaSSEllCapPropertyChoice );
    }
    else if ( device == &m_FeaSSConCapPropertyChoice )
    {
        UpdateCapPropertyIndex( &m_FeaSSConCapPropertyChoice );
    }
    else if ( device == &m_RibPerpendicularEdgeChoice )
    {
        if ( StructureMgr.ValidTotalFeaStructInd( StructureMgr.GetCurrStructIndex() ) )
        {
            vector < FeaStructure* > structVec = StructureMgr.GetAllFeaStructs();

            if ( StructureMgr.GetCurrPartIndex() < structVec[StructureMgr.GetCurrStructIndex()]->NumFeaParts() )
            {
                FeaPart* feaprt = structVec[StructureMgr.GetCurrStructIndex()]->GetFeaPart( StructureMgr.GetCurrPartIndex() );

                if ( feaprt )
                {
                    if ( feaprt->GetType() == vsp::FEA_RIB )
                    {
                        FeaRib* rib = dynamic_cast<FeaRib*>( feaprt );
                        assert( rib );

                        rib->SetPerpendicularEdgeID( m_PerpendicularEdgeVec[m_RibPerpendicularEdgeChoice.GetVal()] );
                    }
                }
            }
        }
    }
    else if ( device == &m_FixPointParentSurfChoice )
    {
        if ( StructureMgr.ValidTotalFeaStructInd( StructureMgr.GetCurrStructIndex() ) )
        {
            vector < FeaStructure* > structVec = StructureMgr.GetAllFeaStructs();

            FeaPart* parent_feaprt = StructureMgr.GetFeaPart( m_FixPointParentIDVec[m_FixPointParentSurfChoice.GetVal()] );

            if ( StructureMgr.GetCurrPartIndex() < structVec[StructureMgr.GetCurrStructIndex()]->NumFeaParts() )
            {
                FeaPart* feaprt = structVec[StructureMgr.GetCurrStructIndex()]->GetFeaPart( StructureMgr.GetCurrPartIndex() );

                if ( feaprt && parent_feaprt )
                {
                    if ( feaprt->GetType() == vsp::FEA_FIX_POINT )
                    {
                        FeaFixPoint* fixpt = dynamic_cast<FeaFixPoint*>( feaprt );
                        assert( fixpt );

                        fixpt->m_ParentFeaPartID = parent_feaprt->GetID();
                    }
                }
            }
        }
    }

    m_ScreenMgr->SetUpdateFlag( true );
}

void FeaPartEditScreen::CloseCallBack( Fl_Widget *w )
{
    assert( m_ScreenMgr );

    Hide();
}

void FeaPartEditScreen::UpdateFeaPropertyChoice()
{
    //==== Property Choice ====//
    m_SkinPropertyChoice.ClearItems();
    m_SlicePropertyChoice.ClearItems();
    m_RibPropertyChoice.ClearItems();
    m_SparPropertyChoice.ClearItems();
    m_FeaSSLinePropertyChoice.ClearItems();
    m_FeaSSRecPropertyChoice.ClearItems();
    m_FeaSSEllPropertyChoice.ClearItems();
    m_FeaSSConPropertyChoice.ClearItems();

    m_SparCapPropertyChoice.ClearItems();
    m_SliceCapPropertyChoice.ClearItems();
    m_RibCapPropertyChoice.ClearItems();

    m_FeaSSLineCapPropertyChoice.ClearItems();
    m_FeaSSRecCapPropertyChoice.ClearItems();
    m_FeaSSEllCapPropertyChoice.ClearItems();
    m_FeaSSConCapPropertyChoice.ClearItems();

    Vehicle*  veh = m_ScreenMgr->GetVehiclePtr();

    if ( veh )
    {
        vector< FeaProperty* > property_vec = StructureMgr.GetFeaPropertyVec();

        for ( int i = 0; i < property_vec.size(); ++i )
        {
            m_SkinPropertyChoice.AddItem( string( property_vec[i]->GetName() ) );
            m_SlicePropertyChoice.AddItem( string( property_vec[i]->GetName() ) );
            m_RibPropertyChoice.AddItem( string( property_vec[i]->GetName() ) );
            m_SparPropertyChoice.AddItem( string( property_vec[i]->GetName() ) );
            m_FeaSSLinePropertyChoice.AddItem( string( property_vec[i]->GetName() ) );
            m_FeaSSRecPropertyChoice.AddItem( string( property_vec[i]->GetName() ) );
            m_FeaSSEllPropertyChoice.AddItem( string( property_vec[i]->GetName() ) );
            m_FeaSSConPropertyChoice.AddItem( string( property_vec[i]->GetName() ) );

            m_SparCapPropertyChoice.AddItem( string( property_vec[i]->GetName() ) );
            m_SliceCapPropertyChoice.AddItem( string( property_vec[i]->GetName() ) );
            m_RibCapPropertyChoice.AddItem( string( property_vec[i]->GetName() ) );

            m_FeaSSLineCapPropertyChoice.AddItem( string( property_vec[i]->GetName() ) );
            m_FeaSSRecCapPropertyChoice.AddItem( string( property_vec[i]->GetName() ) );
            m_FeaSSEllCapPropertyChoice.AddItem( string( property_vec[i]->GetName() ) );
            m_FeaSSConCapPropertyChoice.AddItem( string( property_vec[i]->GetName() ) );

            if ( property_vec[i]->m_FeaPropertyType() == SHELL_PROPERTY )
            {
                m_SkinPropertyChoice.SetFlag( i, 0 );
                m_SlicePropertyChoice.SetFlag( i, 0 );
                m_RibPropertyChoice.SetFlag( i, 0 );
                m_SparPropertyChoice.SetFlag( i, 0 );
                m_FeaSSLinePropertyChoice.SetFlag( i, 0 );
                m_FeaSSRecPropertyChoice.SetFlag( i, 0 );
                m_FeaSSEllPropertyChoice.SetFlag( i, 0 );
                m_FeaSSConPropertyChoice.SetFlag( i, 0 );

                m_SparCapPropertyChoice.SetFlag( i, FL_MENU_INACTIVE );
                m_SliceCapPropertyChoice.SetFlag( i, FL_MENU_INACTIVE );
                m_RibCapPropertyChoice.SetFlag( i, FL_MENU_INACTIVE );

                m_FeaSSLineCapPropertyChoice.SetFlag( i, FL_MENU_INACTIVE );
                m_FeaSSRecCapPropertyChoice.SetFlag( i, FL_MENU_INACTIVE );
                m_FeaSSEllCapPropertyChoice.SetFlag( i, FL_MENU_INACTIVE );
                m_FeaSSConCapPropertyChoice.SetFlag( i, FL_MENU_INACTIVE );
            }
            else if ( property_vec[i]->m_FeaPropertyType() == BEAM_PROPERTY )
            {
                m_SkinPropertyChoice.SetFlag( i, FL_MENU_INACTIVE );
                m_SlicePropertyChoice.SetFlag( i, FL_MENU_INACTIVE );
                m_RibPropertyChoice.SetFlag( i, FL_MENU_INACTIVE );
                m_SparPropertyChoice.SetFlag( i, FL_MENU_INACTIVE );
                m_FeaSSLinePropertyChoice.SetFlag( i, FL_MENU_INACTIVE );
                m_FeaSSRecPropertyChoice.SetFlag( i, FL_MENU_INACTIVE );
                m_FeaSSEllPropertyChoice.SetFlag( i, FL_MENU_INACTIVE );
                m_FeaSSConPropertyChoice.SetFlag( i, FL_MENU_INACTIVE );

                m_SparCapPropertyChoice.SetFlag( i, 0 );
                m_SliceCapPropertyChoice.SetFlag( i, 0 );
                m_RibCapPropertyChoice.SetFlag( i, 0 );

                m_FeaSSLineCapPropertyChoice.SetFlag( i, 0 );
                m_FeaSSRecCapPropertyChoice.SetFlag( i, 0 );
                m_FeaSSEllCapPropertyChoice.SetFlag( i, 0 );
                m_FeaSSConCapPropertyChoice.SetFlag( i, 0 );
            }
        }

        m_SkinPropertyChoice.UpdateItems();
        m_SlicePropertyChoice.UpdateItems();
        m_RibPropertyChoice.UpdateItems();
        m_SparPropertyChoice.UpdateItems();
        m_FeaSSLinePropertyChoice.UpdateItems();
        m_FeaSSRecPropertyChoice.UpdateItems();
        m_FeaSSEllPropertyChoice.UpdateItems();
        m_FeaSSConPropertyChoice.UpdateItems();

        m_SparCapPropertyChoice.UpdateItems();
        m_SliceCapPropertyChoice.UpdateItems();
        m_RibCapPropertyChoice.UpdateItems();

        m_FeaSSLineCapPropertyChoice.UpdateItems();
        m_FeaSSRecCapPropertyChoice.UpdateItems();
        m_FeaSSEllCapPropertyChoice.UpdateItems();
        m_FeaSSConCapPropertyChoice.UpdateItems();

        if ( StructureMgr.ValidTotalFeaStructInd( StructureMgr.GetCurrStructIndex() ) )
        {
            vector < FeaStructure* > structvec = StructureMgr.GetAllFeaStructs();

            if ( StructureMgr.GetCurrPartIndex() < structvec[StructureMgr.GetCurrStructIndex()]->NumFeaParts() )
            {
                FeaPart* feaprt = structvec[StructureMgr.GetCurrStructIndex()]->GetFeaPart( StructureMgr.GetCurrPartIndex() );

                if ( feaprt )
                {
                    // Update all FeaPart Property Choices ( Only Selected Part Visible )
                    m_SkinPropertyChoice.SetVal( feaprt->GetFeaPropertyIndex() );
                    m_SlicePropertyChoice.SetVal( feaprt->GetFeaPropertyIndex() );
                    m_RibPropertyChoice.SetVal( feaprt->GetFeaPropertyIndex() );
                    m_SparPropertyChoice.SetVal( feaprt->GetFeaPropertyIndex() );

                    m_SparCapPropertyChoice.SetVal( feaprt->GetCapFeaPropertyIndex() );
                    m_SliceCapPropertyChoice.SetVal( feaprt->GetCapFeaPropertyIndex() );
                    m_RibCapPropertyChoice.SetVal( feaprt->GetCapFeaPropertyIndex() );
                }
            }
            else if ( StructureMgr.GetCurrPartIndex() >= structvec[StructureMgr.GetCurrStructIndex()]->NumFeaParts() )
            {
                SubSurface* subsurf = structvec[StructureMgr.GetCurrStructIndex()]->GetFeaSubSurf( StructureMgr.GetCurrPartIndex() - structvec[StructureMgr.GetCurrStructIndex()]->NumFeaParts() );

                if ( subsurf )
                {
                    // Update all FeaSubSurface Property Choices ( Only Selected Part Visible )
                    m_FeaSSLinePropertyChoice.SetVal( subsurf->GetFeaPropertyIndex() );
                    m_FeaSSRecPropertyChoice.SetVal( subsurf->GetFeaPropertyIndex() );
                    m_FeaSSEllPropertyChoice.SetVal( subsurf->GetFeaPropertyIndex() );
                    m_FeaSSConPropertyChoice.SetVal( subsurf->GetFeaPropertyIndex() );

                    m_FeaSSLineCapPropertyChoice.SetVal( subsurf->GetCapFeaPropertyIndex() );
                    m_FeaSSRecCapPropertyChoice.SetVal( subsurf->GetCapFeaPropertyIndex() );
                    m_FeaSSEllCapPropertyChoice.SetVal( subsurf->GetCapFeaPropertyIndex() );
                    m_FeaSSConCapPropertyChoice.SetVal( subsurf->GetCapFeaPropertyIndex() );
                }
            }
        }
    }
}

void FeaPartEditScreen::UpdateFixPointParentChoice()
{
    //==== FixPoint Parent Surf Choice ====//
    m_FixPointParentSurfChoice.ClearItems();
    m_FixPointParentIDVec.clear();

    if ( StructureMgr.ValidTotalFeaStructInd( StructureMgr.GetCurrStructIndex() ) )
    {
        vector< FeaStructure* > structVec = StructureMgr.GetAllFeaStructs();
        vector<FeaPart*> feaprt_vec = structVec[StructureMgr.GetCurrStructIndex()]->GetFeaPartVec();

        for ( size_t i = 0; i < feaprt_vec.size(); i++ )
        {

            if ( !structVec[StructureMgr.GetCurrStructIndex()]->FeaPartIsFixPoint( i ) )
            {
                m_FixPointParentSurfChoice.AddItem( string( feaprt_vec[i]->GetName() + "_Surf" ) );
                m_FixPointParentIDVec.push_back( feaprt_vec[i]->GetID() );
            }
        }

        m_FixPointParentSurfChoice.UpdateItems();

        if ( StructureMgr.GetCurrPartIndex() < structVec[StructureMgr.GetCurrStructIndex()]->NumFeaParts() )
        {
            FeaPart* feaprt = structVec[StructureMgr.GetCurrStructIndex()]->GetFeaPart( StructureMgr.GetCurrPartIndex() );

            if ( feaprt )
            {
                if ( feaprt->GetType() == vsp::FEA_FIX_POINT )
                {
                    FeaFixPoint* fixpt = dynamic_cast<FeaFixPoint*>( feaprt );
                    assert( fixpt );

                    FeaPart* parent_feaprt = StructureMgr.GetFeaPart( fixpt->m_ParentFeaPartID );

                    if ( parent_feaprt )
                    {
                        int parent_index = structVec[StructureMgr.GetCurrStructIndex()]->GetFeaPartIndex( parent_feaprt );

                        if ( structVec[StructureMgr.GetCurrStructIndex()]->ValidFeaPartInd( parent_index ) )
                        {
                            m_FixPointParentSurfChoice.SetVal( parent_index );
                        }
                    }
                    else
                    {
                        // Set skin as parent surface if undefined
                        if ( structVec[StructureMgr.GetCurrStructIndex()]->GetFeaSkin() )
                        {
                            fixpt->m_ParentFeaPartID = structVec[StructureMgr.GetCurrStructIndex()]->GetFeaSkin()->GetID();
                        }
                    }
                }
            }
        }
    }
}

void FeaPartEditScreen::UpdatePerpendicularRibChoice()
{
    //==== Perpendicular Rib Choice ====//
    m_RibPerpendicularEdgeChoice.ClearItems();
    m_PerpendicularEdgeVec.clear();

    m_RibPerpendicularEdgeChoice.AddItem( "None" );
    m_PerpendicularEdgeVec.push_back( "None" );
    m_RibPerpendicularEdgeChoice.AddItem( "Leading Edge" );
    m_PerpendicularEdgeVec.push_back( "Leading Edge" );
    m_RibPerpendicularEdgeChoice.AddItem( "Trailing Edge" );
    m_PerpendicularEdgeVec.push_back( "Trailing Edge" );

    Vehicle*  veh = m_ScreenMgr->GetVehiclePtr();

    if ( veh )
    {
        if ( StructureMgr.ValidTotalFeaStructInd( StructureMgr.GetCurrStructIndex() ) )
        {
            FeaStructure* curr_struct = StructureMgr.GetAllFeaStructs()[StructureMgr.GetCurrStructIndex()];

            if ( curr_struct )
            {
                vector < FeaPart* > part_vec = curr_struct->GetFeaPartVec();

                for ( size_t i = 1; i < part_vec.size(); i++ )
                {
                    if ( part_vec[i]->GetType() == vsp::FEA_SPAR )
                    {
                        m_RibPerpendicularEdgeChoice.AddItem( part_vec[i]->GetName() );
                        m_PerpendicularEdgeVec.push_back( part_vec[i]->GetID() );
                    }
                }

                m_RibPerpendicularEdgeChoice.UpdateItems();

                if ( StructureMgr.GetCurrPartIndex() < curr_struct->NumFeaParts() )
                {
                    FeaPart* feaprt = curr_struct->GetFeaPart( StructureMgr.GetCurrPartIndex() );

                    if ( feaprt )
                    {
                        if ( feaprt->GetType() == vsp::FEA_RIB )
                        {
                            FeaRib* rib = dynamic_cast<FeaRib*>( feaprt );
                            assert( rib );

                            for ( size_t k = 0; k < m_PerpendicularEdgeVec.size(); k++ )
                            {
                                if ( rib->GetPerpendicularEdgeID() == m_PerpendicularEdgeVec[k] )
                                {
                                    m_RibPerpendicularEdgeChoice.SetVal( k );
                                    rib->SetPerpendicularEdgeIndex( k );
                                    break;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

void FeaPartEditScreen::FeaPartDispGroup( GroupLayout* group )
{
    if ( m_CurFeaPartDispGroup == group && group )
    {
        return;
    }

    m_SkinEditLayout.Hide();
    m_SliceEditLayout.Hide();
    m_SliceEditLayout.Hide();
    m_RibEditLayout.Hide();
    m_SparEditLayout.Hide();
    m_FixPointEditLayout.Hide();

    m_FeaSSLineGroup.Hide();
    m_FeaSSRecGroup.Hide();
    m_FeaSSEllGroup.Hide();
    m_FeaSSConGroup.Hide();

    m_CurFeaPartDispGroup = group;

    if ( group )
    {
        group->Show();
    }
}

void FeaPartEditScreen::UpdateFeaPartPropertyIndex( Choice* property_choice )
{
    if ( StructureMgr.ValidTotalFeaStructInd( StructureMgr.GetCurrStructIndex() ) )
    {
        vector < FeaStructure* > structvec = StructureMgr.GetAllFeaStructs();

        if ( StructureMgr.GetCurrPartIndex() < structvec[StructureMgr.GetCurrStructIndex()]->NumFeaParts() )
        {
            FeaPart* feaprt = structvec[StructureMgr.GetCurrStructIndex()]->GetFeaPart( StructureMgr.GetCurrPartIndex() );

            if ( feaprt )
            {
                feaprt->SetFeaPropertyIndex( property_choice->GetVal() );
            }
        }
        else if ( StructureMgr.GetCurrPartIndex() >= structvec[StructureMgr.GetCurrStructIndex()]->NumFeaParts() )
        {
            SubSurface* subsurf = structvec[StructureMgr.GetCurrStructIndex()]->GetFeaSubSurf( StructureMgr.GetCurrPartIndex() - structvec[StructureMgr.GetCurrStructIndex()]->NumFeaParts() );

            if ( subsurf )
            {
                subsurf->SetFeaPropertyIndex( property_choice->GetVal() );
            }
        }
    }
}

void FeaPartEditScreen::UpdateCapPropertyIndex( Choice* property_choice )
{
    if ( StructureMgr.ValidTotalFeaStructInd( StructureMgr.GetCurrStructIndex() ) )
    {
        vector < FeaStructure* > structvec = StructureMgr.GetAllFeaStructs();

        if ( StructureMgr.GetCurrPartIndex() < structvec[StructureMgr.GetCurrStructIndex()]->NumFeaParts() )
        {
            FeaPart* feaprt = structvec[StructureMgr.GetCurrStructIndex()]->GetFeaPart( StructureMgr.GetCurrPartIndex() );

            if ( feaprt )
            {
                feaprt->SetCapFeaPropertyIndex( property_choice->GetVal() );
            }
        }
        else if ( StructureMgr.GetCurrPartIndex() >= structvec[StructureMgr.GetCurrStructIndex()]->NumFeaParts() )
        {
            SubSurface* subsurf = structvec[StructureMgr.GetCurrStructIndex()]->GetFeaSubSurf( StructureMgr.GetCurrPartIndex() - structvec[StructureMgr.GetCurrStructIndex()]->NumFeaParts() );

            if ( subsurf )
            {
                subsurf->SetCapFeaPropertyIndex( property_choice->GetVal() );
            }
        }
    }
}

void FeaPartEditScreen::UpdateUnitLabels()
{
    Vehicle* veh = m_ScreenMgr->GetVehiclePtr();

    if ( veh )
    {
        string mass_unit;

        switch ( veh->m_StructMassUnit() )
        {
        case vsp::MASS_UNIT_G:
        mass_unit = "g";
        break;

        case vsp::MASS_UNIT_KG:
        mass_unit = "kg";
        break;

        case vsp::MASS_UNIT_TONNE:
        mass_unit = "t"; // or Mg/
        break;

        case vsp::MASS_UNIT_LB:
        mass_unit = "lb";
        break;

        case vsp::MASS_UNIT_SLUG:
        mass_unit = "slug";
        break;
        }

        m_FixPointMassUnit.GetFlButton()->copy_label( mass_unit.c_str() );
    }
}