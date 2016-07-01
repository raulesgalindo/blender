/*
 * Copyright 2011-2013 Blender Foundation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

CCL_NAMESPACE_BEGIN

/* Attribute Node */

ccl_device void svm_node_attr_init(KernelGlobals *kg, ShaderData *sd,
	uint4 node, NodeAttributeType *type,
	AttributeDescriptor *desc, uint *out_offset)
{
	*out_offset = node.z;
	*type = (NodeAttributeType)node.w;

	if(ccl_fetch(sd, object) != OBJECT_NONE) {
		if(find_attribute(kg, sd, node.y, desc) == ATTR_STD_NOT_FOUND) {
			desc->element = ATTR_ELEMENT_NONE;
			desc->offset = 0;
			desc->type = (NodeAttributeType)node.w;
		}
	}
	else {
		/* background */
		desc->element = ATTR_ELEMENT_NONE;
		desc->offset = 0;
		desc->type = (NodeAttributeType)node.w;
	}
}

ccl_device void svm_node_attr(KernelGlobals *kg, ShaderData *sd, float *stack, uint4 node)
{
	NodeAttributeType type;
	AttributeDescriptor desc;
	uint out_offset;

	svm_node_attr_init(kg, sd, node, &type, &desc, &out_offset);

	/* fetch and store attribute */
	if(type == NODE_ATTR_FLOAT) {
		if(desc.type == NODE_ATTR_FLOAT) {
			float f = primitive_attribute_float(kg, sd, &desc, NULL, NULL);
			stack_store_float(stack, out_offset, f);
		}
		else {
			float3 f = primitive_attribute_float3(kg, sd, &desc, NULL, NULL);
			stack_store_float(stack, out_offset, average(f));
		}
	}
	else {
		if(desc.type == NODE_ATTR_FLOAT3) {
			float3 f = primitive_attribute_float3(kg, sd, &desc, NULL, NULL);
			stack_store_float3(stack, out_offset, f);
		}
		else {
			float f = primitive_attribute_float(kg, sd, &desc, NULL, NULL);
			stack_store_float3(stack, out_offset, make_float3(f, f, f));
		}
	}
}

ccl_device void svm_node_attr_bump_dx(KernelGlobals *kg, ShaderData *sd, float *stack, uint4 node)
{
	NodeAttributeType type;
	AttributeDescriptor desc;
	uint out_offset;

	svm_node_attr_init(kg, sd, node, &type, &desc, &out_offset);

	/* fetch and store attribute */
	if(type == NODE_ATTR_FLOAT) {
		if(desc.type == NODE_ATTR_FLOAT) {
			float dx;
			float f = primitive_attribute_float(kg, sd, &desc, &dx, NULL);
			stack_store_float(stack, out_offset, f+dx);
		}
		else {
			float3 dx;
			float3 f = primitive_attribute_float3(kg, sd, &desc, &dx, NULL);
			stack_store_float(stack, out_offset, average(f+dx));
		}
	}
	else {
		if(desc.type == NODE_ATTR_FLOAT3) {
			float3 dx;
			float3 f = primitive_attribute_float3(kg, sd, &desc, &dx, NULL);
			stack_store_float3(stack, out_offset, f+dx);
		}
		else {
			float dx;
			float f = primitive_attribute_float(kg, sd, &desc, &dx, NULL);
			stack_store_float3(stack, out_offset, make_float3(f+dx, f+dx, f+dx));
		}
	}
}

ccl_device void svm_node_attr_bump_dy(KernelGlobals *kg, ShaderData *sd, float *stack, uint4 node)
{
	NodeAttributeType type;
	AttributeDescriptor desc;
	uint out_offset;

	svm_node_attr_init(kg, sd, node, &type, &desc, &out_offset);

	/* fetch and store attribute */
	if(type == NODE_ATTR_FLOAT) {
		if(desc.type == NODE_ATTR_FLOAT) {
			float dy;
			float f = primitive_attribute_float(kg, sd, &desc, NULL, &dy);
			stack_store_float(stack, out_offset, f+dy);
		}
		else {
			float3 dy;
			float3 f = primitive_attribute_float3(kg, sd, &desc, NULL, &dy);
			stack_store_float(stack, out_offset, average(f+dy));
		}
	}
	else {
		if(desc.type == NODE_ATTR_FLOAT3) {
			float3 dy;
			float3 f = primitive_attribute_float3(kg, sd, &desc, NULL, &dy);
			stack_store_float3(stack, out_offset, f+dy);
		}
		else {
			float dy;
			float f = primitive_attribute_float(kg, sd, &desc, NULL, &dy);
			stack_store_float3(stack, out_offset, make_float3(f+dy, f+dy, f+dy));
		}
	}
}

CCL_NAMESPACE_END

