
#include "enginesetupbase.h"

class EngineSetupDialog : public EngineSetupDialogBase
{
	Q_OBJECT
	
	public:
		EngineSetupDialog(QWidget* parent = 0);
    ~EngineSetupDialog();

	public slots:
		void slotListChanged(QListBoxItem*);
		void slotNameChanged(const QString& name);
		void slotCommandChanged(const QString& command);
		void slotProtocolChanged(int);
		void slotBrowse();
		void slotDelete();
		void slotNew();

	private:
		typedef enum { Winboard, UCI } Protocol;	

		void removeEntry(const QString& engineName);
		void writeEntry(const QString& engineName, const QString& command, Protocol protocol);
		void setControlsEnabled(bool enabled);

		int m_engine;
		QString m_name;
		QString m_command;
		Protocol m_protocol;
};
