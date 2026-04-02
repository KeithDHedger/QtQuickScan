
#ifndef _QT_PREFSWIDGETS_
#define _QT_PREFSWIDGETS_

enum whatWidget{EDITWIDGET,COLOURWIDGET,FONTWIDGET,CHECKWIDGET,COMBOWIDGET,SPINWIDGET};

struct	holdData
{
	whatWidget	what=EDITWIDGET;
	QWidget		*widget=NULL;
	QString		name="";
};

class	prefsWidgetsClass
{
	public:
		prefsWidgetsClass(QString prefsname,QWidget *parent=nullptr);
		prefsWidgetsClass();
		~prefsWidgetsClass();

		QDialog					*window=NULL;
		bool						getAsFolder=false;

		void						setPrefsName(QString name);
		void						startWindow(QString wname);
		void						addWidgetToWindow(QWidget *widget);
		void						finishWindow(bool addclear=false);

//colour
		QWidget					*getPrefsColourWidget(QString label,QString defaultcolour);
		QString					getColourValue(QString label);
		void						setColourValue(QString label);
		QString					getColourPref(QString name);
		void						setColourPref(QString name,QString prefsval);
		void						setAllColourPrefs(void);

//check
		QWidget					*getPrefsCheckWidget(QString label,bool defaultstate);
		bool						getCheckValue(QString label);
		void						setCheckValue(QString label);
		bool						getCheckPref(QString name);
		void						setCheckPref(QString name,bool prefsval);
		void						setAllCheckPrefs(void);

//combo
		QWidget					*getPrefsComboWidget(QString label,QStringList data,QString item="");
		QString					getComboValue(QString label);
		void						setComboValue(QString label);
		QString					getComboPref(QString name);
		void						setComboPref(QString name,QString prefsval);
		void						setAllComboPrefs(void);

//font
		QWidget					*getPrefsFontWidget(QString label,QString defaultfont);
		QString					getFontValue(QString label);
		void						setFontValue(QString label);
		QString					getFontPref(QString name);
		void						setFontPref(QString name,QString prefsval);
		void						setAllFontPrefs(void);

//edits
		QWidget					*getPrefsEditWidget(QString label,QString defaulttext);
		QString					getEditValue(QString label);
		void						setEditValue(QString label);
		QString					getEditPref(QString name);
		void						setEditPref(QString name,QString prefsval);
		void						setAllEditPrefs(void);

//spinner
		QWidget					*getPrefsSpinWidget(QString label,int min,int max,int val);
		int						getSpinValue(QString label);
		void						setSpinValue(QString label);
		int						getSpinPref(QString name);
		void						setSpinPref(QString name,int prefsval);
		void						setAllSpinPrefs(void);

//file
		QWidget					*getPrefsFileWidget(QString label,QString path,QString openhere="");
		QString					getFileValue(QString label);
		void						setFileValue(QString label);
		QString					getFilePref(QString name);
		void						setFilePref(QString name,QString prefsval);
		void						setAllFilePrefs(void);

	private:
		QWidget					*getCheckWidget(QString label,bool state);
		QWidget					*getEditWidget(QString label,QString text);
		QWidget					*getFileWidget(QString label,QString path,QString openhere="");
		QWidget					*getSpinWidget(QString label,int min,int max,int val);
		QWidget					*getFontWidget(QString label,QString font);
		QWidget					*getComboWidget(QString label,QStringList data,QString item="");
		QWidget					*getColourWidget(QString label,QString colour);

		QHash<QString,QString>	colours;
		QHash<QString,QString>	combos;
		QHash<QString,QString>	fonts;
		QHash<QString,QString>	edits;
		QHash<QString,QString>	files;
		QHash<QString,int>		spins;
		QHash<QString,bool>		checks;

		QVector<holdData>		widgets;

		int						labelWidth=100;
		QWidget					*theParent=nullptr;
		QString					thePrefsName;
		QVBoxLayout				*windowVLayout;

		bool						deletePrefs(void);
		void						setLabelWidth(int sze);
		QString					bestFontColour(QString colour);
		void						setPref(QHash<QString,QString> what,QString label);
		void						setAllPrefs(QHash<QString,QString> what);
		QWidget					*getPrefsWidget(QString label,QString defaulttxt,whatWidget what);

		QString					normalizeLabel(QString label);
		QString					getStringPref(QString name);
		void						setStringValue(QString name,QString prefsval);
};

#endif
