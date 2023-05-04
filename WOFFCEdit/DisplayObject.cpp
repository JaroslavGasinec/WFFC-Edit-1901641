#include "DisplayObject.h"

DisplayObject::DisplayObject()
{
	m_model = NULL;
	m_texture_diffuse = NULL;
	m_orientation.x = 0.0f;
	m_orientation.y = 0.0f;
	m_orientation.z = 0.0f;
	m_position.x = 0.0f;
	m_position.y = 0.0f;
	m_position.z = 0.0f;
	m_scale.x = 0.0f;
	m_scale.y = 0.0f;
	m_scale.z = 0.0f;
	m_render = true;
	m_wireframe = false;

	m_light_type =0;
	m_light_diffuse_r = 0.0f;	m_light_diffuse_g = 0.0f;	m_light_diffuse_b = 0.0f;
	m_light_specular_r = 0.0f;	m_light_specular_g = 0.0f;	m_light_specular_b = 0.0f;
	m_light_spot_cutoff = 0.0f;
	m_light_constant = 0.0f;
	m_light_linear = 0.0f;
	m_light_quadratic = 0.0f;
}


DisplayObject::~DisplayObject()
{
//	delete m_texture_diffuse;
}

void DisplayObject::Move(const Vector3 &worldOffset)
{
	m_position += worldOffset;
	m_pendingCommit = true;
}

void DisplayObject::Rotate(const Vector3 &offsetRotation)
{
	m_orientation += offsetRotation;
	m_pendingCommit = true;
}

void DisplayObject::Scale(const Vector3& offsetScale)
{
	m_scale += offsetScale;
	m_pendingCommit = true;
}

void DisplayObject::Reset()
{
	m_scale = Vector3(1, 1, 1);
	m_orientation = Vector3(0, 0, 0);
}

void DisplayObject::MarkSelected()
{
	m_isSelected = true;
	m_model->UpdateEffects([&](IEffect* highlightEffect)
		{
			auto* fogEffect = dynamic_cast<DirectX::IEffectFog*>(highlightEffect);
		
			if (fogEffect)
			{
				fogEffect->SetFogEnabled(true);
				fogEffect->SetFogColor(DirectX::Colors::YellowGreen);
			}
		});
}

void DisplayObject::UnmarkSelected()
{
	m_isSelected = false;
	m_model->UpdateEffects([&](IEffect* highlightEffect)
		{
			auto* fogEffect = dynamic_cast<DirectX::IEffectFog*>(highlightEffect);

			if (fogEffect)
				fogEffect->SetFogEnabled(false);
		});
}

bool DisplayObject::IsSelected()
{
	return m_isSelected;
}
