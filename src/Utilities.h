#pragma once

// Indices (locations) of QUeue Families (if they exist at all)
class QueueFamilyIndices 
{
	public:
		int graphicsFamily = -1;
		int presentFamily = -1;

		bool isValid()
		{
			return graphicsFamily >= 0 && presentFamily >= 0;
		}

};