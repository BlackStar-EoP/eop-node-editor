#include "NodeGraphicsItem.h"

#include "NodePortGraphicsItem.h"

#include "model/NodePort.h"
#include "model/Node.h"
#include "model/NodeModel.h"

#include <QPainter>
#include <QGraphicsTextItem>

NodeGraphicsItem::NodeGraphicsItem(Node& node)
: m_node(node)
{
	setPos(node.position());
	setFlags(ItemIsMovable | ItemIsSelectable);

	QString name = m_node.title();
	if (m_node.is_orphan())
		name += " (Orphan)";

	QGraphicsTextItem* title = new QGraphicsTextItem(name, this);

	NodeModel* model = m_node.model();

	uint32_t num_ports = model->num_ports();

	for (uint32_t i = 0; i < num_ports; ++i)
	{
		NodePort* port = new NodePort(model->port_model(i), &m_node);
		NodePortGraphicsItem* port_item = new NodePortGraphicsItem(this, *port, i);
	}

	recalculate_size();

	setFlag(QGraphicsItem::ItemSendsGeometryChanges);
}

QRectF NodeGraphicsItem::boundingRect() const
{
	return m_bounding_rect;
}

void NodeGraphicsItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	painter->drawRoundedRect(m_bounding_rect, 10.0f, 10.0f);
}

void NodeGraphicsItem::recalculate_size()
{
	const uint32_t WIDTH = 200;
	const uint32_t HEIGHT = 50;
	const uint32_t PORT_HEIGHT = 25;

	uint32_t port_height = PORT_HEIGHT * m_node.model()->num_ports();
	m_bounding_rect = QRectF(0, 0, WIDTH, port_height + HEIGHT);
}

QVariant NodeGraphicsItem::itemChange(GraphicsItemChange change, const QVariant& value)
{
	if (change == ItemPositionHasChanged)
	{
		m_node.set_position(pos());
	}

	return QGraphicsItem::itemChange(change, value);
}